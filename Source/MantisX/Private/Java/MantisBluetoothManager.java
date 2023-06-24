package com.chimeraxr.mantisapi;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.support.annotation.Keep;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class MantisBluetoothManager
{
    private static final long SCAN_PERIOD = 10000;
    private List<BluetoothDevice> knownDevices = new ArrayList<>();
    private BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    private BluetoothLeScanner scanner = bluetoothAdapter.getBluetoothLeScanner();

    private ScanCallback scanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            if (knownDevices.contains())
        }
    }

    private Activity _activity;
    private boolean isScanning = false;

    @Keep
    public MantisBluetoothManager(Activity activity) {
        _activity = activity;
    }

    @Keep
    public boolean isBluetoothEnabled() {
        if (bluetoothAdapter == null) return false;

        return bluetoothAdapter.isEnabled();
    }

    @Keep
    public boolean doesSupportBle() {
        return _activity.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE);
    }

    private Handler handler = new Handler();

    @Keep
    public void startScanningForBluetoothDevices() {
        // Already scanning
        if (isScanning) return;

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                isScanning = false;
                scanner.stopScan(bleScanCallback);
            }
        }, SCAN_PERIOD);
    }
}