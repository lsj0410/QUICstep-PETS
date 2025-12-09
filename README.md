# Artifact Appendix

Paper title: **QUICstep: Evaluating connection migraton based QUIC censorship circumvention**

Requested Badge(s):
  - [x] **Available**
  - [x] **Functional**
  - [x] **Reproduced**

## Description
This artifact contains the source code for **QUICstep: Evaluating connection migraton based QUIC censorship circumvention** (*Seungju Lee, Mona Wang, Watson Jia, Qiang Wu, Henry Birge-Lee, Liang Wang, and Prateek Mittal*, PoPETS 2026).
QUICstep circumvents QUIC SNI censorship by selectively routing QUIC Initial and Handshake packets over a secure *handshake channel*[^1].
This repo contains the following:

1. QUICstep implementation using a WireGuard channel as a handshake channel (`quicstep_config`').
2. Script and necessary resources for large scale QUIC connection migration measurement (section 4.2 of paper, `app`, `chromium_build`).
3. Raw data from QUICstep performance evaluation (section 4.4 of paper, `performance`).

### Security/Privacy Issues and Ethical Concerns

QUICstep, being a censorship circumvention tool, carries fundamental security risks for users when accessing censored domains.
In our experiments with real-world censors we used a client machine under our control hosted at a large-scale commercial VPS provider with a dedicated IP address to avoid harming real users, and accessed only a limited number of real-world domains to avoid the client machine itself being blocked.

## Basic Requirements

### Hardware Requirements

We assume that the user has access to two machines: Client and Proxy.
The Client machine will act as the QUICstep client, execute QUIC scans, and test performance measurements.
The Proxy machine will act as the QUICstep proxy.
The Client machine must have x86 architecture to be able to run QUIC scans with the Chromium source code.
Beyond this there are no particular requirements for Client/Proxy hardware.

The experiments reported in the paper used AWS EC2 instances and VirtualBox running on laptop as client, and AWS Lightsail instances as proxies.

### Software Requirements

#### QUICstep

Our implementation of QUICstep uses WireGuard as the underlying handshake channel. Further detail about installing WireGuard and using it is described in the Setup section. It is expected that the client machine will have separate private and public IP addresses.

#### Large scale QUIC scans 

The scans reported in section 4.2 ran on Ubuntu 22.04 and Python 3.10, and it requires building Chromium from source. This repo includes pre-built Chromium binaries in `chromium_build`. Required Python packages are included in `requirements.txt`.
An IPASN dataset and Tranco domain list are required to run the code and are included in `app`.

#### Performance measurements

The performance measurements reported in section 4.4 ran on Ubuntu 20.04, because recent versions of Selenium and Chromedriver does not support forcing QUIC on destinations.
This repository includes raw data from the measurements and scripts to reproduce the figures in the paper.
Python packages `selenium`, `numpy`, `matplotlib` are required.

### Estimated Time and Storage Consumption

#### Large scale QUIC scans

While the full scans in the paper tested 1 million domains with 500 workers and took 5+ hours, the code in this repo tests 1000 domains with 10 workers by default and takes about 10 minutes. Reviewers may try different numbers of domains or workers.

## Environment

### Accessibility

Our artifacts will be made accessible at https://github.com/inspire-group/QUICstep-PETS

### Set up the environment

First ensure that UDP ingress traffic on port 51820 is allowed for both client and proxy machines.

Clone this repository at the client and install necessary packages.

```bash
git clone https://github.com/lsj0410/QUICstep-PETS
cd QUICstep-PETS
pip3 install -r requirements.txt
cd quicstep-config
```

On the proxy machine, create a `setup.sh` file and `takedown.sh` file and copy into them the contents of `quicstep-config/setup.sh` and `quicstep-config/takedown.sh`, respectively.

Then on both machines, install WireGuard and generate keys.

```bash
sudo bash setup.sh
sudo apt update
sudo apt install resolvconf wireguard
wg genkey | sudo tee /etc/wireguard/private.key
sudo chmod go= /etc/wireguard/private.key
sudo cat /etc/wireguard/private.key | wg pubkey | sudo tee /etc/wireguard/public.key
```

In the client machine, create `/etc/wireguard/wg_qs.conf` and `/etc/wireguard/wg_vpn.conf`.
Note that `/etc/wireguard` is only accessible in sudo.
The content of these files should correspond to `quicstep-config/client/wg_qs.conf` and `quicstep-config/client/wg_vpn.conf` in this repository, with the keys and IP addresses being those values in your machines.

In the proxy machine, create `/etc/wireguard/wg0.conf`.
The content of this file should correspond to `quicstep-config/proxy/wg0.conf` in this repository, with the keys and IP address being those values in your machines.

### Testing the Environment

#### WireGuard interfaces

These tests test that the WireGuard interfaces are functional, we will look into whether they act as desired in following experiments.

Pull up the WireGuard interface at the proxy.

```bash
wg-quick up wg0
```

Pull up the VPN WireGuard interface at the client.

```bash
wg-quick up wg_vpn
```

Run `sudo tcpdump -i wg0` at the proxy and `dig google.com` at the client. Confirm that DNS traffic is going through the proxy. Run `sudo wg` on both sides and confirm that data transfer has happened.

Pull down the VPN interface, pull up the QUICstep interface.

```bash
wg-quick down wg_vpn
wg-quick up wg_qs
```

Run `sudo tcpdump -i wg0` at the proxy and `./chromium_build/Default/quic_client --quiet www.google.com` at the client. Confirm that (1) some QUIC traffic is going through the proxy, and (2) the chromium binaries are functional at the client. The client terminal output should look like the following.

```bash
Connected to www.google.com:443
Request succeeded (200).
```

Once complete pull down the QUICstep interface with `wg-quick down wg_qs`.

## Artifact Evaluation

### Main Results and Claims

#### Main Result 1: QUIC and connection migration support

Around 22% of domains support QUIC, and around 13% of QUIC supporting domains support connection migration.
Corresponds to Figure 3, Experiment 1.

#### Main Result 2: Page load time

Page load time in QUICstep is significantly shorter than that of VPN.
Corresponds to Figure 4 and Figure 6, Experiment 2.

#### Main Result 3: Time to first byte

Time to first byte in QUICstep is comparable to that of VPN.
Corresponds to Figure 5, Experiment 2.

#### Main Result 4: Proxy load

QUICstep significantly reduces load on proxy compared to VPN.
Corresponds to Figure 7, Experiment 2.

### Experiments

#### Experiment 1: QUIC and connection migration support
- Time: ~10 minutes with default configuration.

```bash
cd app
python3 quic-and-migration-support.py -f random_1.csv
```

Defaults to using 10 workers, can be configured by adding -w [worker-count] when running.
`random_1.csv` and `random_2.csv` each contain 1000 randomly sampled domains from `tranco_20251119.csv`.
The user may also run `python3 quic-and-migration-support.py -d [domain-count]` to sample `domain-count` domains during runtime.
Prints number of QUIC and port migration supporting domains to terminal and outputs a csv file containing results for each domain.

#### Experiment 2: Reproducing figures from raw data
- Time: <5 minutes

This repository includes raw data for Figures 4, 5, 6, 7, 9 in the paper. (Raw data for Figure 8 is included in the Appendix of the paper.) Code used for generating this data is also included.

```bash
cd performance/figure4_7_9
python3 figure4_7_9.py
```

Outputs Figures 4, 7, 9.

```bash
cd ../figure5_6
python3 figure5_6.py
```

Outputs Figures 5, 6.

## Limitations

The data in the performance measurements are unlikely to be exactly reproduced, since latency heavily depends on the location of client and proxy machines. We include the raw data from our measurements instead.

## Notes on Reusability

We would like to emphasize that the idea of QUICstep is not constrained to a particular handshake channel; while we chose WireGuard for our implementation, any secure blocking resistant channel that the client can access for every connection can be a handshake channel.
Our implementation in particular can be altered to support handshake channels that provide a virtual network interface.
We encourage other researchers to create implementations of QUICstep with other secure channels.

[^1]: Any secure, blocking resistant but potentially high-latency channel (e.g. VPN)
