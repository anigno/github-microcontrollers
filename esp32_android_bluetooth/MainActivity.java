import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private Button button1, button2;
    private EditText statusText;
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;
    private Thread thread;

    private static final UUID UUID_BT = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // Standard SerialPortService ID

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button1 = findViewById(R.id.button1);
        button2 = findViewById(R.id.button2);
        statusText = findViewById(R.id.statusText);

        // Initialize Bluetooth
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            showToast("Bluetooth not supported on this device");
            finish();
            return;
        }

        // Connect to ESP32 device
        connectToDevice();

        // Set button click listeners
        button1.setOnClickListener(v -> sendData("Button 1 clicked"));
        button2.setOnClickListener(v -> sendData("Button 2 clicked"));

        // Start thread to receive data from ESP32
        startThread();
    }

    private void connectToDevice() {
        // Replace "ESP32_BT_DEVICE_NAME" with your ESP32 Bluetooth device name
        String deviceName = "ESP32_BT_DEVICE_NAME";

        BluetoothDevice device = null;
        for (BluetoothDevice pairedDevice : bluetoothAdapter.getBondedDevices()) {
            if (pairedDevice.getName().equals(deviceName)) {
                device = pairedDevice;
                break;
            }
        }

        if (device == null) {
            showToast("ESP32 device not found");
            return;
        }

        try {
            bluetoothSocket = device.createRfcommSocketToServiceRecord(UUID_BT);
            bluetoothSocket.connect();
            inputStream = bluetoothSocket.getInputStream();
            outputStream = bluetoothSocket.getOutputStream();
            showToast("Connected to ESP32 device");
        } catch (IOException e) {
            showToast("Failed to connect to ESP32 device: " + e.getMessage());
        }
    }

    private void startThread() {
        thread = new Thread(() -> {
            byte[] buffer = new byte[1024];
            int bytes;

            while (true) {
                try {
                    bytes = inputStream.read(buffer);
                    String data = new String(buffer, 0, bytes);
                    handler.obtainMessage(0, data).sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        });
        thread.start();
    }

    private void sendData(String data) {
        try {
            outputStream.write(data.getBytes());
            showToast("Data sent: " + data);
        } catch (IOException e) {
            showToast("Failed to send data: " + e.getMessage());
        }
    }

    private void showToast(String message) {
        runOnUiThread(() -> Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show());
    }

    private final Handler handler = new Handler(msg -> {
        String receivedData = (String) msg.obj;
        statusText.setText(receivedData);
        return true;
    });

    @Override
    protected void onDestroy() {
        super.onDestroy();
        try {
            if (bluetoothSocket != null) {
                bluetoothSocket.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
