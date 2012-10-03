package com.ahmet.smartwatch;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.widget.Toast;

public class BTservice extends Service {
	private BluetoothAdapter mBluetoothAdapter = null;	
    private static BluetoothSerialService mSerialService = null;   
    private NotificationManager mNM;
    private int NOTIFICATION = R.string.servicestarted;


    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }
    public class LocalBinder extends Binder {
    	BTservice getService() {
            return BTservice.this;
        }
    }
    // This is the object that receives interactions from clients.  See
    // RemoteService for a more complete example.
    private final IBinder mBinder = new LocalBinder();
	
	@Override
	public void onCreate() {



	}

	@Override
	public void onDestroy() {
		Toast.makeText(this, "My Service Stopped", Toast.LENGTH_LONG).show();

	}
	
	@Override
	public void onStart(Intent intent, int startid) {
		IntentFilter filter = new IntentFilter("es.pymasde.blueterm.Disconnect");
	    this.registerReceiver(new Receiver(), filter);
	    
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		if (mBluetoothAdapter == null) {
			return;
		}
		
       
        mSerialService = new BluetoothSerialService(this, mHandlerBT);  
        
        String address = intent.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        // Attempt to connect to the device
        mSerialService.connect(device);  
        Toast.makeText(this, "SmartWatch Service Started", Toast.LENGTH_LONG).show();
        showNotification();

	}
	 private void showNotification() {
	        // In this sample, we'll use the same text for the ticker and the expanded notification
	        CharSequence text = getText(R.string.servicestarted);

	        // Set the icon, scrolling text and timestamp
	        Notification notification = new Notification(R.drawable.icon, text,
	                System.currentTimeMillis());

	        // The PendingIntent to launch our activity if the user selects this notification
	        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
	                new Intent(this, SmartWatch.class), 0);

	        // Set the info for the views that show in the notification panel.
	        notification.setLatestEventInfo(this, "SmartWatch",
	                       text, contentIntent);
	        mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
	        // Send the notification.
	        mNM.notify(NOTIFICATION, notification);
	    }
    private final Handler mHandlerBT = new Handler() {
    	
        @Override
        public void handleMessage(Message msg) { 
        }
    };    
    private class Receiver extends BroadcastReceiver {

    	 @Override
    	 public void onReceive(Context arg0, Intent arg1) {
    	    int message =arg1.getExtras().getInt("request");
    	    switch(message)
    	    {
    	    case 1:
    	    	mSerialService.stop();
    	    	mNM.cancel(NOTIFICATION);
    	    break;
    	    }
    	 }

    	}

}