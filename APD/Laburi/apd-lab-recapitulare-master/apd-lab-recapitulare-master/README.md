# Laborator recapitulare APD

## How to run Java applet

After compiling the code:
```
javac Main.java
```

Run it using:
```
appletviewer -J-Djava.security.policy=applet.policy myapplet.html
```

### If a Java version mismatch error appears

Download the latest version of Adopt Open JDK

```
wget https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u232-b09/OpenJDK8U-jdk_x64_linux_hotspot_8u232b09.tar.gz
```

Extract the archive:
```
tar -xzf OpenJDK8U-jdk_x64_linux_hotspot_8u232b09.tar.gz
```

Enter the `bin` folder of the extracted JDK:

```
cd jdk8u232-b09/bin
```

Add the new version to `PATH`:
```
export PATH=$PWD:$PATH
```

Now recompile and run the Applet ***on the same terminal*** (because the export has only a temporary effect on the current bash session).
