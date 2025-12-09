import argparse
import csv
import os
from datetime import datetime
from datetime import date
from itertools import islice
from threading import Thread
from multiprocessing import Pool
from multiprocessing import cpu_count
import subprocess
import pandas as pd
import socket
import pyasn

asndb = pyasn.pyasn('ipasn_20251119.dat')
trancofile = 'tranco_20251119.csv'
date = str(date.today())
otxt = f'output_port_{date}.txt'

parser = argparse.ArgumentParser()
parser.add_argument('--domain_count', '-d',
                    type=int,
                    help='number of domains to test; default 1K',
                    default=1000)
parser.add_argument('--worker_count', '-w',
                    type=int,
                    help='number of workers; default 10',
                    default=10)
args = parser.parse_args()
num = args.domain_count
workers = args.worker_count

def test_h3(row):

    line = str(row[0]) + '\t' + row[1] 
    result = [[False, False], [False, False]]

    try:
        ip = socket.gethostbyname(row[1])
        asn = asndb.lookup(ip)
    except socket.error as e:
        ip = 'Error'
        asn = 'Error'

    process = None

    def quic_get():

        # Test both example.com and www.example.com
        www = ['', 'www.']

        for i in [0, 1]:
            domain = f'https://{www[i]}{row[1]}'
            nonlocal process
            process_list = ['/home/ubuntu/Default/quic_client',
                            '--disable_certificate_verification',
                            '--quiet',
                            '--num_requests=2',
                            domain]
            if ip != 'Error':
                process_list.append(f'--host={ip}')

            try:
                process = subprocess.Popen(
                    process_list,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                )
                stdout, stderr = process.communicate()
                stdout = stdout.decode('utf-8').strip()
                stderr = stderr.decode('utf-8').strip()
            except Exception as e:
                print('Error: ', e)

            if stdout:
                result[0][i] = True
                if len(stdout) >= 34:
                    if 'Request failed' in stdout[-34:-2] or 'Segmentation fault' in stdout[-34:-2]:
                        result[0][i] = False
                        continue
                if result[0][i] == True and stderr == '':
                    result[1][i] = True
                    break
        
    thread = Thread(target = quic_get)
    thread.start()
    thread.join(timeout = 500)

    if thread.is_alive():
        if process:
            process.kill()
            thread.join()

    h3 = str(result[0][0] | result[0][1])
    cm = str(result[1][0] | result[1][1])

    line = f'{line}\t{h3}\t{cm}\t{ip}\t{asn}'
    print(line, flush = True)
    append = open(otxt, 'a')
    append.write(f'{line}\n')
    append.close()

    return

if __name__ == '__main__':

    start = datetime.now()
    
    write = open(otxt, 'w')
    write.write('Num\tDomain\tQUIC\tConnection migration\tIP\tASN\n')
    write.close()
    
    with open(trancofile) as csvfile:
        csv_reader = csv.reader(csvfile)
        top_rows = list(islice(csv_reader, num))
        pool = Pool(workers)
        h3_domains = pool.map(test_h3, top_rows)

    df_output = pd.read_csv(otxt, delimiter = '\t')
    df_output = df_output.sort_values(by = 'Num')

    df_output['ASN_Clean'] = df_output['ASN'].str.extract('\((\d+),*').fillna('')

    quic = df_output['QUIC'].value_counts()[True]
    cm = df_output['Connection migration'].value_counts()[True]
    print(f'QUIC support: {quic}')
    print(f'Connection migration support: {cm}')

    df_output.to_csv(f'quic_and_port_migration_{date}.csv', index = False)
    os.remove(otxt)
    end = datetime.now()
    print(end - start)