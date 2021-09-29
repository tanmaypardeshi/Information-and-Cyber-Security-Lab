import java.math.BigInteger;
import java.util.Random;

public class RsaUtil
{
    private final BigInteger N;
    private final BigInteger e;
    private final BigInteger d;

    public BigInteger getN() {
        return N;
    }

    public BigInteger getE() {
        return e;
    }

    public BigInteger getD() {
        return d;
    }

    public RsaUtil()
    {
        Random r = new Random();
        int bitlength = 1024;
        BigInteger p = BigInteger.probablePrime(bitlength, r);
        BigInteger q = BigInteger.probablePrime(bitlength, r);
        N = p.multiply(q);
        BigInteger phi = p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));
        e = BigInteger.probablePrime(bitlength / 2, r);
        while (phi.gcd(e).compareTo(BigInteger.ONE) > 0 && e.compareTo(phi) < 0)
        {
            e.add(BigInteger.ONE);
        }
        d = e.modInverse(phi);
    }

    public String bytesToString(byte[] encrypted)
    {
        StringBuilder test = new StringBuilder();
        for (byte b : encrypted)
        {
            test.append(Byte.toString(b));
        }
        return test.toString();
    }

    // Encrypt message
    public byte[] encrypt(byte[] message, BigInteger e, BigInteger N)
    {
        return (new BigInteger(message)).modPow(e, N).toByteArray();
    }

    // Decrypt message
    public byte[] decrypt(byte[] message, BigInteger d, BigInteger N)
    {
        return (new BigInteger(message)).modPow(d, N).toByteArray();
    }
}