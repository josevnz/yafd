#!/usr/bin/env python
"""
Script to expose Arduino serial port readings using Prometheus client code.
Author: Jose Vicente Nunez (kodegeek.com@protonmail.com)
"""
from argparse import ArgumentParser
import serial
from prometheus_client import Gauge, Info
from prometheus_client import start_wsgi_server
from rich.console import Console
from rich.table import Table

BAUD = 9600
TIMEOUT = 2
SERIAL_PORT = "/dev/ttyUSB0"
PROMETHEUS_AGENT_PORT = 8000
VERSION = "0.0.1"


def capture(sp: serial.Serial, gauge: Gauge, verbose: bool) -> None:
    while True:
        if serial_port.in_waiting > 0:
            serial_string = serial_port.readline()
            data_line = serial_string.decode('utf-8')
            distance_in_cm = data_line.split()[0].strip()
            try:
                cm_gauge.set(float(distance_in_cm))
                if ARGS.verbose:
                    console.log(f"[green]{distance_in_cm}[/green]")
            except ValueError:
                if ARGS.verbose:
                    console.print(f"[red]Skipping:[/red] '{data_line}'")


if __name__ == "__main__":

    PARSER = ArgumentParser(__doc__)
    PARSER.add_argument('--serial_port', action='store', default=SERIAL_PORT, help=f"Serial port name {SERIAL_PORT}")
    PARSER.add_argument('--baud', action='store', default=str(BAUD), help=f"Serial port speed {BAUD}")
    PARSER.add_argument('--prom_port', action='store', default=PROMETHEUS_AGENT_PORT,
                        help=f"Prometheus {PROMETHEUS_AGENT_PORT}")
    PARSER.add_argument('--verbose', action='store_true', default=False, help=f"Print captured reading to screen")
    ARGS = PARSER.parse_args()
    try:
        serial_port = serial.Serial(port=ARGS.serial_port, baudrate=BAUD, bytesize=8, timeout=TIMEOUT,
                                    stopbits=serial.STOPBITS_ONE)
        yafd_prom_info = Info('prometheus_yafd_exporter', 'Share metrics about YAFD')
        yafd_prom_info.info({'version': VERSION, 'baud': ARGS.baud, 'port': ARGS.serial_port})
        cm_gauge = Gauge('distance', 'yafd_obstacle_distance_in_cm')
        start_wsgi_server(port=ARGS.prom_port)
        if ARGS.verbose:
            console = Console()
            with console.status("Working..."):
                console.rule("[bold red]Starting capture][/bold red]")
                capture(sp=serial_port, gauge=cm_gauge, verbose=ARGS.verbose)
        else:
            capture(sp=serial_port, gauge=cm_gauge, verbose=ARGS.verbose)
    except KeyboardInterrupt:
        pass
