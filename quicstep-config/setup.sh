#!/bin/bash

# enable IPv4 forwarding
sysctl -w net.ipv4.ip_forward=1

# disable IPv6
sysctl -w net.ipv6.conf.all.disable_ipv6=1
sysctl -w net.ipv6.conf.default.disable_ipv6=1
sysctl -w net.ipv6.conf.lo.disable_ipv6=1