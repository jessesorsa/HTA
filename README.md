# Welcome to Hiking Tour Assistant (HTA) git repository.

This project was done as part of Aalto University course ELEC-E8408 - Embedded Systems Development.

Authors: **Jesse Sorsa, Kalle Lindgren, Santeri Piironen, Zhibin Yan**.


## What is HTA

Hiking Tour Assistant is system designed to promote outdoor activities by providing interesting insigts into hikings sessions.

HTA tracks user during a hiking session with a Smartwatch.
Tracked items are steps, distance, and time taken during the hike. These are displayed to the user in real-time.
Supported smartwatch is LILYGO T-WATCH 2020 V2.  

HTA also provides possibility to store session data for later usage in a central Hub.
Sessions are synchronized to the Hub automatically when the Smartwatch is within Bluetooth range of the Hub.
Saved sessions are prosessed to include burned calories estimation.
RaspberryPi 3b+ was used as the Hub.

Saved sessions are accessable through a Web UI.
Users are able to view summary information of previous sessios and delete any saved session.
This Web UI is hosted locally in the Hub, and thus accessable in same LAN as the Hub.


### Repo structure

Main directory contains files needed for running the HUB.

frontend subdirectory contains code for the Web UI.

watch subdirectory contains code for the watch.

### See SRS for more information about the project requirements.
### Design report details current implementation and helps if you wish to understand it in more detail.
