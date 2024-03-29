# Welcome to Hiking Tour Assistant (HTA) git repository.

This project was done as part of Aalto University course ELEC-E8408 - Embedded Systems Development.

Authors: **Jesse Sorsa, Kalle Lindgren, Santeri Piironen, Zhibin Yan**.


## What is HTA

Hiking Tour Assistant is a system designed to promote outdoor activities by providing interesting insights into hiking sessions.

HTA tracks users during a hiking session with a Smartwatch.
Tracked items are steps, distance, and time taken during the hike. These are displayed to the user in real-time.
Supported smartwatch is LILYGO T-WATCH 2020 V2.  

HTA also provides the possibility of storing session data for later usage in a central Hub.
Sessions are synchronized to the Hub automatically when the Smartwatch is within the Hub's Bluetooth range.
Saved sessions are processed to include the estimation of burned calories.
Raspberry Pi 3b+ was used as the Hub.

Saved sessions are accessible through a Web UI.
Users are able to view summary information of previous sessions and delete any saved session.
This Web UI is hosted locally in the Hub and thus accessible in the same LAN as the Hub.


### Repo structure

The main directory contains files needed to run the Hub.

frontend subdirectory contains code for the Web UI.

watch subdirectory contains code for the watch.

### See SRS for more information about the project requirements.
### The design report details the current implementation and helps if you wish to understand it in more detail.

## Hub installation guide

### Dependencies

**Bluetooth for Python**

To install `pybluez` on an RPi for the Bluetooth connection, install the following dependencies.
```sh
sudo apt-get install libbluetooth-dev
sudo apt-get install python-dev
sudo pip install PyBluez
```

**Other dependencies**

The necessary Python libraries can be installed from the `requirements.txt` file.
Assuming we are in the project folder; run the following:
`pip3 install -r HTA/requirements.txt`

### Installing the Hub

```bash
git clone https://github.com/jessesorsa/HTA
```

### Initializing the Hub
```bash
cd HTA
bash HTA/run.sh
```
