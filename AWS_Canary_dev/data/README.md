## Paste the following security certificates here:

* AmazonRootCA1 (PEM file)

* Certificate (DER file*)

* Private (DER file*)


## To convert .pem to .der:
* Install [OpenSSL on Windows](https://www.youtube.com/watch?v=TjNq7lFt-t0)
* Go to your C:\certificates directory in cmd 
* Open openssl :
`c:\OpenSSL-Win32\bin\openssl.exe`

* **Converting: pem to der**, type in the following:  
    `x509 -outform der -in ex12345-certificate.pem.crt -out cert.der`

    `rsa -outform der -in ex12345-private.pem.key -out private.der`


