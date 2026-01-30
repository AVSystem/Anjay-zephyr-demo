# Anjay-zephyr-demo [<img align="right" height="50px" src="https://avsystem.github.io/Anjay-doc/_images/avsystem_logo.png">](http://www.avsystem.com/)

## Overview

This repository contains LwM2M Client samples based on open-source
[Anjay Zephyr Module](https://github.com/AVSystem/Anjay-zephyr-module) and
[nRF Connect SDK](https://github.com/nrfconnect/sdk-nrf) v.3.2.0.

This project works on Nordic Semiconductor nRF9160DK.


## Getting started

First of all, get Zephyr, SDK and other dependencies, as described in Zephyr's
[Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html)
(first 4 steps). Also for flashing
[nRF Util](https://www.nordicsemi.com/Products/Development-tools/nRF-Util)
have to be installed.

After navigating to Zephyr workspace (`~/zephyrproject` is default after following Getting Started Guide), clone Anjay Zephyr client repository.
```
git clone https://github.com/AVSystem/Anjay-zephyr-demo
```

and then download and/or update the demo app dependencies according to west manifest
```
west config manifest.path Anjay-zephyr-demo
west config manifest.file west-nrf.yml
west update
```


## Configuration

Anjay is configured by files found `config/`, exported as globally visible
in Zephyr build system by `CMakeLists.txt`.

Check the [online documentation](https://avsystem.github.io/Anjay-doc/api/anjay__config_8h.html)
or see the files' inline comments to learn how to configure Anjay capabilities and behavior.

Anjay as Zephyr module can be enabled and configured with `Kconfig`. Run
```
west build -t menuconfig
```
and see `Anjay zephyr demo settings` and `Zephyr Kernel -> Modules -> anjay-zephyr-module` menus.


## Build and run demo

Now, enter the demo app directory and compile the application
```
cd Anjay-zephyr-demo
west build -b nrf9160dk/nrf9160/ns -p
```

To flash the board, run
```
west flash
```


### Serial Output

The application logs are sent over the USB interface at **115200 baud** (8N1).
The nRF9160DK exposes multiple ports. Use the one corresponding to **VCOM0** (usually `/dev/ttyACM0` on Linux).


## License

This repository is licensed under the **Apache License 2.0** and is intended for **evaluation and prototyping purposes**.

Please note that it is a demo based on Anjay Zephyr Module, which includes the Anjay LwM2M Client SDK as a Git submodule.
Anjay Lite is distributed under a dual-license model:

A **non-commercial license** is available for evaluation and non-commercial use.
For **commercial use or production deployments**, a valid **per-SKU commercial license** is required.

For details, see the [Anjay LICENSE file](https://github.com/AVSystem/Anjay/blob/master/LICENSE) or visit [avsystem.com](https://www.avsystem.com/products/anjay/).
