package com.example.zhro.wifi;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.WifiP2pManager.Channel;
import android.util.Log;

public class WiFiDirectBroadcastReceiver extends BroadcastReceiver {

	private static final String TAG = "ZHRO";
	private WifiP2pManager mManager;
	private Channel mChannel;
	private MyWiFiActivity mActivity;

	public WiFiDirectBroadcastReceiver(WifiP2pManager mManager,
			Channel mChannel, MyWiFiActivity mActivity) {
		super();
		this.mManager = mManager;
		this.mChannel = mChannel;
		this.mActivity = mActivity;
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		
        if (WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)) {
            int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);
            if (state == WifiP2pManager.WIFI_P2P_STATE_ENABLED) {
                // Wifi P2P is enabled
            	Log.i(TAG, "WiFiDirectBroadcastReceiver---Wifi P2P is enabled");
            } else {
                // Wi-Fi P2P is not enabled
            	Log.i(TAG, "WiFiDirectBroadcastReceiver---Wi-Fi P2P is not enabled");
            }
        } else if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)) {
        	
        } else if (WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.equals(action)) {
        	
        } else if (WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION.equals(action)) {
        	
        }
	}

}
