package com.ahmet.smartwatch;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;


public class SmartWatch extends Activity {
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
    private static TextView mTitle;
    private Button connectButton;
    
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        setContentView(R.layout.main);
        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title);

        mTitle = (TextView) findViewById(R.id.title_left_text);
        mTitle.setText(R.string.app_name);
        mTitle = (TextView) findViewById(R.id.title_right_text);       

        connectButton=(Button)findViewById(R.id.button1);
        connectButton.setOnClickListener(new OnClickListener()
        {
			public void onClick(View v) {
				connectButton();
			}    
        });
        Button temp=(Button)findViewById(R.id.button8);
        temp.setOnClickListener(new OnClickListener()
        {
			public void onClick(View v) {
				stopService();
			}    
        }); 
	}
	public void connectButton()
	{
		Intent serverIntent = new Intent(this, DeviceListActivity.class);
		startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
	}
	public void stopService()
	{
		Intent intent=new Intent();
		intent.setAction("es.pymasde.blueterm.Disconnect");
		intent.putExtra("request",1);
		sendBroadcast(intent);		
	}
	@Override
	public void onDestroy() {
		super.onDestroy();
		stopService();        
	}
    
    public void finishDialogNoBluetooth() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage(R.string.alert_dialog_no_bt)
        .setIcon(android.R.drawable.ic_dialog_info)
        .setTitle(R.string.app_name)
        .setCancelable( false )
        .setPositiveButton(R.string.alert_dialog_ok, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                       finish();            	
                	   }
               });
        AlertDialog alert = builder.create();
        alert.show(); 
    }
    
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {        
        case REQUEST_CONNECT_DEVICE:
            if (resultCode == Activity.RESULT_OK) {

                String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);              
                Intent btService=new Intent(this, BTservice.class);
                btService.putExtra(DeviceListActivity.EXTRA_DEVICE_ADDRESS, address);
                startService(btService);              
            }
            break;

        case REQUEST_ENABLE_BT:
            if (resultCode == Activity.RESULT_OK) {                
                finishDialogNoBluetooth();                
            }
        }
    }  
}
