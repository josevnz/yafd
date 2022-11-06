Yet Another Food Dispenser (YAFD)

A small project/ tutorial I put together while learning how to use Influxdb for data processing.

Please read the [Tutorial](TUTORIAL.md) file for details.

Arduino sketch files and Python code is provided, along with screenshots and schematics to build this project.

# Python Developer mode

```shell
python -m venv ~/virtualenv/yafd
. ~/virtualenv/yafd/bin/activate
pip install --upgrade pip
pip install --upgrade setuptools
pip install --upgrade build
pip install --upgrade wheel
python setup.py develop
```

And if you want to build a wheel to install somewhere else

```shell
. ~/virtualenv/yafd/bin/activate
python -m build --wheel .
```
