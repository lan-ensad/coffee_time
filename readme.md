# Coffee Machine

## RaspberryPi

### Setup Wifi wlan0

edit wpa_supplicant.conf to setup wifi.

`sudo nano /etc/network/wpa_supplicant/wpa_supplicant.conf`

```block
country=FR
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
ap_scan=1

update_config=1
network={
    scan_ssid=1
    ssid="SSId"
    psk="PASS"
}
```

`ap_scan=1` is mendatory for hidden ssid network. 

### Install Grafana port :3000

[based on Grafana documentation](https://grafana.com/tutorials/install-grafana-on-raspberry-pi/)

**add :**

`wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -`

`echo "deb https://packages.grafana.com/oss/deb stable main" | sudo tee -a /etc/apt/sources.list.d/grafana.list`

**install :**

`sudo apt update && sudo apt install -y grafana`

`sudo systemctl enable grafana-server && sudo systemctl start grafana-server`

**test Grafana UI :**

`http://<ip address>:3000`

### Install InfluxDB port :8086

[based on hackster.io](https://www.hackster.io/kamaluddinkhan/how-to-set-up-influxdb-2-on-a-raspberry-pi-b8ef84)

[Influx documentation](https://docs.influxdata.com/influxdb/v2.6/install/)

**add :**

`wget -qO- https://repos.influxdata.com/influxdb.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/influxdb.gpg > /dev/null`

`export DISTRIB_ID=$(lsb_release -si); export DISTRIB_CODENAME=$(lsb_release -sc)`

`echo "deb [signed-by=/etc/apt/trusted.gpg.d/influxdb.gpg] https://repos.influxdata.com/${DISTRIB_ID,,} ${DISTRIB_CODENAME} stable" | sudo tee /etc/apt/sources.list.d/influxdb.list > /dev/null`

**install :**

`sudo apt update && sudo apt install influxdb2 influxdb2-cli -y`

`sudo systemctl enable influxdb && sudo systemctl start influxdb`

**test Influx UI:**

`http://<ip address>:8086`

### Add Influx in Grafana Data Source

connect to Grafana UI in a web browser :

⚙️→ Data Sources → Add data source → "InfluxDB"
<div style="width:100px">IDENTIFIER</div>|<div style="width:150px">VALUE</div>|
--|--|
name            |what you want                      |
language        |InfluxQL                           |
URL             |http://< ipaddress >:8086          |
Basic auth ✅   | with credentials ✅              |
User            |  go to influxdb → About → User ID |
Password        | Password of this user             |
HTTP Headers    | Add new|
Header          | `Authorization`                   |
Value           | `Token Wh_a6EjGQvkwvTaAeVE7qq                         AAvhYo4Z491ai25gtjjKoX_CV99zZ_zyBObn9SHKxaBwrPsfHRwCdos5tBAtgPu6g==`      |
Database        | < name of bucket >                |
User            | go to influxdb → About → User ID  |
Password        | Password of this user             |
HTTP Methode    | `GET`                             |

### Embedded Grafana on Web Page

**install certbot :**

`sudo apt update && sudo apt install certbot python3-certbot-apache software-properties-common -y`


**Setup Grafana :**

allow anonymous connection

`sudo nano /etc/grafana/grafana.ini --line` → ctrl+w `auth.anonymous`

`allow_embedding = true` → L.300

```
[auth.anonymous]
enabled = true

org_name = < you org >

org_role = Viewer
```

## ESP

[link here](https://randomnerdtutorials.com/esp8266-nodemcu-influxdb/)

`http://<ip address>:8086` Data → Sources → Arduino

`http://<ip address>:8086` Data → API Tokens `VmKT8a91HltXeKR5LTSQdfSI9NneqmHc8fsGKqZRkTuYTMXQI8wAFLB1z7iBYp-JnAn1NIi6ZJCdPDnSEbwr7w==`