import java.io.*;
import java.net.*;
import java.math.BigInteger;

class RSA_Client {
    public static void main(String[] args) throws IOException, UnknownHostException {

        RsaUtil rsaUtil = new RsaUtil();
        BigInteger d = rsaUtil.getD();
        BigInteger e = rsaUtil.getE();
        BigInteger n = rsaUtil.getN();

        Socket cs = new Socket("LAPTOP-GA8C0AII", 5003);

        // reserve memory for communication with server
        BufferedReader fromclient = new BufferedReader(new InputStreamReader(System.in));
        BufferedReader fromserver = new BufferedReader(new InputStreamReader(cs.getInputStream()));
        DataOutputStream toserver_byte = new DataOutputStream(cs.getOutputStream());
        DataInputStream fromserver_byte = new DataInputStream(cs.getInputStream());
        PrintWriter toserver = new PrintWriter(cs.getOutputStream(), true);

        // Once connected, the client sees a welcome message on his machine and he tends to respond and the conversation
        // begins
        String from_server_d_string = fromserver.readLine();
        String from_server_n_string = fromserver.readLine();

        BigInteger from_server_d = new BigInteger(from_server_d_string);
        BigInteger from_server_n = new BigInteger(from_server_n_string);

        System.out.println("\nReceived public key from server\n");

        toserver.println(d);
        toserver.println(n);

        while(true) {
            int length = fromserver_byte.readInt();
            if(length > 0) {
                byte[] fromserver_enc = new byte[length];
                fromserver_byte.readFully(fromserver_enc, 0, fromserver_enc.length);
                System.out.println("Server:- " + fromserver_enc);
                byte[] decrypted = rsaUtil.decrypt(fromserver_enc, e, n);
                String decrypted_string = rsaUtil.bytesToString(decrypted);
                System.out.println("Decrypted Server:- " + new String(decrypted));
                if(new String(decrypted).equalsIgnoreCase("bye")) {
                    break;
                }
            }
            System.out.print("Client:- ");
            String s = fromclient.readLine();
            byte[] encrypted = rsaUtil.encrypt(s.getBytes(), from_server_d, from_server_n);
            System.out.println("Encrypted Client:- " + encrypted);
            toserver_byte.writeInt(encrypted.length);
            toserver_byte.write(encrypted);
        }

        toserver.close();
        fromserver.close();
        fromclient.close();
        cs.close();
    }
}
