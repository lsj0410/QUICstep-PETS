import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def figure4():
  df = pd.read_csv('top100domains.csv')
  x1 = df['loadtime_vpn']
  y1 = df['loadtime_quicstep']

  plt.figure(figsize=(10, 6))
  plt.rcParams['pdf.fonttype'] = 42
  plt.rcParams['font.family'] = 'Times New Roman'
  plt.rcParams['font.size'] = 30

  plt.scatter(x1, y1, s=100, edgecolor='black', facecolor='none', label='Rate limit 5Mbps', alpha=0.6)

  x_values = np.linspace((min(x1), max(x1)), 100)
  plt.plot(x_values, x_values, 'k--', label='y = x')

  plt.xlabel('VPN page load time (ms)')
  plt.ylabel('QUICstep page \n load time (ms)')
  plt.xlim(50, None)
  plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
  plt.tight_layout()
  plt.savefig('figure4.pdf', bbox_inches='tight', format='pdf')
  plt.close()

def figure7():
  df = pd.read_csv('top100domains.csv')
  data = df['Proxy load QUICstep/VPN'].values
  base = np.sort(data)
  cdf = np.linspace(0, 1, len(base), endpoint=True)
  median = np.median(data)

  plt.rcParams['pdf.fonttype'] = 42
  plt.rcParams['font.family'] = 'Times New Roman'
  plt.rcParams['font.size'] = 30
  plt.figure(figsize=(10,6))

  plt.xlabel('Traffic ratio')
  plt.ylabel('CDF')
  plt.plot(base, cdf, linestyle="solid", color="black")
  plt.ylim(0, 1)
  plt.text(median + 0.03, 0.45, f'Median: {median:.2f}',color='black',ha='left',va='bottom')
  plt.text(median + 0.03, 0.35, f'93% load reduction',color='red',ha='left',va='bottom')
  plt.scatter(median + 0.003, 0.5, color = 'black', zorder=10)
  plt.grid()
  plt.savefig('figure7.pdf', bbox_inches='tight', format='pdf')
  plt.close()

def figure9():
  data = pd.read_csv('ratelimit_client_firstbyte.csv')

  x1 = data['VPN_5']
  y1 = data['QUICstep_5']
  x2 = data['VPN_10']
  y2 = data['QUICstep_10']
  x3 = data['VPN']
  y3 = data['QUICstep']

  plt.figure(figsize=(12, 8))
  plt.rcParams['pdf.fonttype'] = 42
  plt.rcParams['font.family'] = 'Times New Roman'
  plt.rcParams['font.size'] = 30

  plt.scatter(x1, y1, s=100, edgecolor='black', facecolor='none', label='Rate limit 5 Mbps', alpha=0.6)
  plt.scatter(x2, y2, marker='^', edgecolor='black', facecolor = 'none', label='Rate limit 10 Mbps', alpha=0.6, s=100)
  plt.scatter(x3, y3, marker='x', color='black', s=100, label = 'No rate limit', alpha = 0.7) 

  x_values = np.linspace(min(min(x1), min(x2), min(x3)), max(max(x1), max(x2), max(x3)), 100)
  plt.plot(x_values, x_values, 'k--', label='y = x')

  plt.xlabel('VPN time to first byte (ms)')
  plt.ylabel('QUICstep time to \n first byte (ms)')
  plt.axhline(0, color='black',linewidth=0.5, ls='--')
  plt.axvline(0, color='black',linewidth=0.5, ls='--')
  plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
  plt.legend()
  plt.tight_layout()
  plt.savefig('figure9.pdf', bbox_inches='tight', format='pdf')
  plt.close()

if __name__ == '__main__':
  figure4()
  figure7()
  figure9()
