import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

website = 'wwwyoutube'
client_list = ['london', 'osaka', 'nj']
proxy_list = ['ohio', 'ireland', 'seoul']

def draw_cdf_fig5(loc = 'london'):
    fig, axs = plt.subplots(1, 3, figsize=(10,3))
    for i in range(3):
        proxy = proxy_list[i]

        naive = pd.read_csv(f'{website}_{loc}_{proxy}_firstbyte_naive.csv').values.ravel()
        quicstep = pd.read_csv(f'{website}_{loc}_{proxy}_firstbyte_quicstep.csv').values.ravel()
        vpn = pd.read_csv(f'{website}_{loc}_{proxy}_firstbyte_vpn.csv').values.ravel()
        
        naive_base = np.sort(naive)
        naive_cdf = np.linspace(0, 1, len(naive_base), endpoint=True)

        quicstep_base = np.sort(quicstep)
        quicstep_cdf = np.linspace(0, 1, len(quicstep_base), endpoint=True)
        
        vpn_base = np.sort(vpn)
        vpn_cdf = np.linspace(0, 1, len(vpn_base), endpoint=True)

        axs[i].plot(naive_base, naive_cdf, label='Native', color ='g', linestyle='--')
        axs[i].plot(quicstep_base, quicstep_cdf, label='QUICstep', color ='r', linestyle='-')
        axs[i].plot(vpn_base, vpn_cdf, label='VPN', color='b', linestyle=':')
        axs[i].set_ylim(ymin=0, ymax=1)
        axs[i].set_title(proxy, y=-0.3)
    handles, labels = axs[0].get_legend_handles_labels()

    fig.legend(
        handles, labels,
        loc='upper center',
        ncol=len(labels),       # put labels in one row
        bbox_to_anchor=(0.5, 1)  # adjust height above plots
    )
    fig.tight_layout()
    fig.subplots_adjust(top=0.85)
    fig.savefig('figure5.pdf', bbox_inches='tight', format='pdf')

def draw_cdf_fig6():
    fig, axs = plt.subplots(3, 3, figsize=(10,5))
    for i in range(3):
        for j in range(3):
            loc = client_list[i]
            proxy = proxy_list[j]

            naive = pd.read_csv(f'{website}_{loc}_{proxy}_loadtime_naive.csv').values.ravel()
            quicstep = pd.read_csv(f'{website}_{loc}_{proxy}_loadtime_quicstep.csv').values.ravel()
            vpn = pd.read_csv(f'{website}_{loc}_{proxy}_loadtime_vpn.csv').values.ravel()
            
            naive_base = np.sort(naive)
            naive_cdf = np.linspace(0, 1, len(naive_base), endpoint=True)

            quicstep_base = np.sort(quicstep)
            quicstep_cdf = np.linspace(0, 1, len(quicstep_base), endpoint=True)
            
            vpn_base = np.sort(vpn)
            vpn_cdf = np.linspace(0, 1, len(vpn_base), endpoint=True)

            axs[i][j].plot(naive_base, naive_cdf, label='Native', color ='g', linestyle='--')
            axs[i][j].plot(quicstep_base, quicstep_cdf, label='QUICstep', color ='r', linestyle='-')
            axs[i][j].plot(vpn_base, vpn_cdf, label='VPN', color='b', linestyle=':')
            axs[i][j].set_ylim(ymin=0, ymax=1)
            if j == 0:
                axs[i][j].set_ylabel(f'{loc}')
            if i == 2:
                axs[i][j].set_xlabel(f'{proxy}')

    handles, labels = axs[0][0].get_legend_handles_labels()

    fig.legend(
        handles, labels,
        loc='upper center',
        ncol=len(labels),
        bbox_to_anchor=(0.5, 1)
    )
    fig.tight_layout()
    fig.subplots_adjust(top=0.85)
    fig.savefig('figure6.pdf', bbox_inches='tight', format='pdf')

if __name__ == '__main__':
    plt.rcParams['pdf.fonttype'] = 42
    plt.rcParams['font.family'] = 'Times New Roman'
    plt.rcParams['font.size'] = 12
    draw_cdf_fig5()
    draw_cdf_fig6()