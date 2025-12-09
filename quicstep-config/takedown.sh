#!/bin/bash

# disable IPv4 forwarding
sysctl -w net.ipv4.ip_forward=0

# enable IPv6
sysctl -w net.ipv6.conf.all.disable_ipv6=0
sysctl -w net.ipv6.conf.default.disable_ipv6=0
sysctl -w net.ipv6.conf.lo.disable_ipv6=0