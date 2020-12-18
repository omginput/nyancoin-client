#!/usr/bin/env python3

# Nyancoin peer list downloader, by u/nametone.
# V1, May 21, 2020.
# Public domain.

from bs4 import BeautifulSoup
import requests
import re

print("Downloading webpage...")
page = requests.get("https://www.nyanchain.com/pr.nyan")
print("Downloading done")
soup = BeautifulSoup(page.content, 'html.parser')
table = soup.select_one("table")
ipv4=re.compile("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$")
file=open("peers.conf","w")
file.write("addnode=46.235.227.47\n") #Add jwflame's node
for td in table.find_all("td"):
    if ipv4.match(td.text):
        file.write("addnode="+td.text+"\n")
file.close()
print("Writing config done")
print("You can now add peers.conf's content to your nyancoin.conf configuration file.")
