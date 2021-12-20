import java.io.*;
import java.net.*;
import java.math.BigInteger;

class RSA_Server {

    public static void main(String[] args) throws IOException
    {
        // Generate key pairs for server
        RsaUtil rsaUtil = new RsaUtil();
        BigInteger d = rsaUtil.getD();
        BigInteger e = rsaUtil.getE();
        BigInteger n = rsaUtil.getN();

        // Create socket
        ServerSocket ss = new ServerSocket(5003);
        Socket cs = ss.accept();

        // Reserve memory on server for sending and receiving messages
        BufferedReader fromserver = new BufferedReader(new InputStreamReader(System.in));
        BufferedReader fromclient = new BufferedReader(new InputStreamReader(cs.getInputStream()));
        DataInputStream fromclient_byte = new DataInputStream(cs.getInputStream());
        DataOutputStream toclient_byte = new DataOutputStream(cs.getOutputStream());
        PrintWriter toclient = new PrintWriter(cs.getOutputStream(), true);

        // Exchange of public keys
        toclient.println(d);
        toclient.println(n);

        String from_client_d_string = fromclient.readLine();
        String from_client_n_string = fromclient.readLine();

        BigInteger from_client_d = new BigInteger(from_client_d_string);
        BigInteger from_client_n = new BigInteger(from_client_n_string);

        System.out.println("\nReceived public key from client\n");

        // The client has communicated, so send a welcome message to him
        String message = "Hi client";
        byte[] encrypted = rsaUtil.encrypt(message.getBytes(), from_client_d, from_client_n);
        System.out.println("Encryped Server:- " + encrypted);
        toclient_byte.writeInt(encrypted.length);
        toclient_byte.write(encrypted);

        while(true) {
            int length = fromclient_byte.readInt();
            if(length > 0) {
                byte[] fromclient_enc = new byte[length];
                fromclient_byte.readFully(fromclient_enc, 0, fromclient_enc.length);
                System.out.println("Encrypted Client:- " + fromclient_enc);
                byte[] decrypted = rsaUtil.decrypt(fromclient_enc, e, n);
                String decrypted_string = rsaUtil.bytesToString(decrypted);
                System.out.println("Decrypted Client:- " + new String(decrypted));
                if(new String(decrypted).equalsIgnoreCase("bye")) {
                    break;
                }
                System.out.print("Server:- ");
                String s = fromserver.readLine();
                encrypted = rsaUtil.encrypt(s.getBytes(), from_client_d, from_client_n);
                System.out.println("Encrypted Server:- " + encrypted);
                toclient_byte.writeInt(encrypted.length);
                toclient_byte.write(encrypted);
            }
        }


        toclient.close();
        fromclient.close();
        cs.close();
        ss.close();
    }
}
