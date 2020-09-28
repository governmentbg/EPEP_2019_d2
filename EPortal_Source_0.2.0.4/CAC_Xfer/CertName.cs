using System;
using System.IO;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;

public class CertName
{
    public string Name;
    public string Store;  // must be lower case
    public string File = "";  // not recommended

    public X509Certificate2 Find(string password)
    {
        X509Certificate2 certificate = null;
        string publicKey = null;

        if (Name.Length != 0)
        {
            for (StoreLocation location = StoreLocation.LocalMachine; location >= StoreLocation.CurrentUser; location--)
            {
                foreach (StoreName storeName in StoreNames)
                {
                    X509Store store = new X509Store(storeName, location);

                    store.Open(OpenFlags.ReadOnly);

                    try
                    {
                        foreach (X509Certificate2 certificate2 in store.Certificates)
                        {
                            if (certificate2.GetNameInfo(X509NameType.SimpleName, false) == Name &&
                                (String.IsNullOrEmpty(Store) || storeName.ToString().ToLower() == Store))
                            {
                                if (certificate == null)
                                {
                                    certificate = certificate2;
                                    publicKey = Utility.HexString(certificate.GetPublicKey());
                                }
                                else if (Utility.HexString(certificate2.GetPublicKey()) != publicKey)
                                    throw new RangeException("Ambiguous certificate '{0}'.", Name);
                            }
                        }
                    }
                    finally
                    {
                        store.Close();
                    }
                }
            }

            if (certificate == null)
                throw new RangeException("Certificate '{0}' not found.", Name);
        }
        else if (File.Length != 0)
            certificate = new X509Certificate2(File, password);

        return certificate;
    }

    private static StoreName[] StoreNames = { StoreName.AddressBook, StoreName.AuthRoot, StoreName.CertificateAuthority,
        StoreName.Disallowed, StoreName.My, StoreName.Root, StoreName.TrustedPeople, StoreName.TrustedPublisher };
}
