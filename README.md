# X-CASH Proof of Stake - Website Server

This is the server for the [X-CASH Proof of Stake - Website](https://github.com/X-CASH-official/XCASH_proof_of_stake_website)

## Installation

This program will only run on a Linux/Unix OS at this time. We recommend installing this on a Ubuntu VPS/Server (16.04 or 18.04) with at least 1GB of RAM, for the best compatibility.

You will also need to run an X-CASH Daemon and X-CASH RPC wallet on the server. You can either download the latest X-CASH [release](https://github.com/X-CASH-official/X-CASH/releases) or build from [source](https://github.com/X-CASH-official/X-CASH#compiling-x-cash-from-source)



## Compiling X-CASH Proof of Stake - Website Server from source



### Dependencies

The following table summarizes the tools and libraries required to build. 

| Dependencies                                 | Min. version  | Ubuntu package            |
| -------------------------------------------- | ------------- | ------------------------- |
| GCC                                          | 4.7.3         | `build-essential`         | 
| CMake                                        | 3.0.0         | `cmake`                   | 
| pkg-config                                   | any           | `pkg-config`              | 
| OpenSSL      		                       | any           | `libssl-dev`              |
| Git      		                       | any           | `git`                     |
| MongoDB                                      | 4.0.3         |  install from binaries    |
| MongoDB C Driver (includes BSON libary)      | 1.13.1        |  build from source        |
| Libmicrohttpd                                | any           | `libmicrohttpd-dev`       |



### Installing MongoDB from binaries

Visit [https://www.mongodb.com/download-center/community](https://www.mongodb.com/download-center/community)

Then choose your OS, and make sure the version is the current version and the package is server. Then click on All version binaries. Now find the current version to download. You do not want the debug symbols or the rc version, just the regular current version.

Once you have downloaded the file move the file to a location where you want to keep the binaries, then run this set of commands  
`tar -xf mongodb-linux-x86_64-*.tgz && rm mongodb-linux-x86_64-*.tgz && sudo mkdir -p /data/db && sudo chmod 770 /data/db && sudo chown $USER /data/db`



### Building the MongoDB C driver from source

Visit the offical websites installation instructions at [http://mongoc.org/libmongoc/current/installing.html](http://mongoc.org/libmongoc/current/installing.html)
You will need to follow the instructions for [Building from a release tarball](http://mongoc.org/libmongoc/current/installing.html#building-from-a-release-tarball) or [Building from git](http://mongoc.org/libmongoc/current/installing.html#building-from-git) since you need the header files, not just the library files.

After you have built the MongoDB C driver from source, you will need to run  
`sudo ldconfig`



### Adding MongoDB to your PATH
You will probably want to add MongoDB to your path so you can run MonogDB by typing `mongod` at any terminal.

To add MongoDB to your PATH (replace "MongoDB_folder" with the location of the bin folder in the folder you installed MongoDB in  
`echo -e '\nexport PATH=MongoDB_folder:$PATH' >> ~/.profile && source ~/.profile`



### Cloning the repository

`git clone https://github.com/X-CASH-official/XCASH_proof_of_stake_website_server.git`



### Build instructions

X-CASH Proof of Stake - Website Server uses a Make file.

After cloning the repository, navigate to the folder  
`cd XCASH_proof_of_stake_website_server`

Then use the make file to build the binary file  
`make clean ; make`



## Running MongoDB
To run MongoDB you will need to navigate to the folder you downloaded the binaries to, and in the bin folder run mongod by running  
`./mongod`

If you have already added MongoDB to your path, you can just type in any terminal
`mongod`



## Setting up the xcashd and xcash-wallet-RPC

First you will need to run xcashd in the background. Navigate to the folder that contains the xcash binaries, then run  
`./xcashd`

Next you need to run a xcash-wallet-rpc. The server is setup to use port 18285, so we recommend to run the xcash-wallet-RPC on this port.

To run the rpc wallet you can run  
`./xcash-wallet-rpc --wallet-file NAME_OF_WALLET_FILE --password WALLET_FILE_PASSWORD --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon`

Just replace NAME_OF_WALLET_FILE with the name of your wallet file and WALLET_FILE_PASSWORD with the password of that wallet file. Make sure to use port 18285 as this is the port that is used in the program.

We suggest you use the screen command to run the program in the background, this way you can still maintenance the server. To do this run  
`screen -dmS Daemon ./xcashd`

You can also run the RPC wallet this way as well  
`screen -dmS RPC-Wallet ./xcash-wallet-rpc --wallet-file NAME_OF_WALLET_FILE --password WALLET_FILE_PASSWORD --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon`

To bring the screen from the bacground process to the active process run  
`screen -x NAME_OF_BACKGROUNDS_SCREEN`

Where NAME_OF_BACKGROUNDS_SCREEN would be Daemon or RPC-Wallet in the above examples.

To exit a screen if it is the active process, you can press Control + C. To exit a screen that is a background process you can run  
`screen -XS NAME_OF_BACKGROUNDS_SCREEN quit`



## Running X-CASH Proof of Stake - Website Server test
It is recomeneded to run the X-CASH Proof of Stake - Website Server test before you run the main program. The test will ensure that your system is compatbile, and that you have setup your system correctly.

To run the X-CASH Proof of Stake - Website Server test, Navigate to the folder that contains the binary, then run  
`./xcash_proof_of_stake_website_server --test`

The test will return the number of passed and failed test on the bottom of the console. The failed test need to be 0 before you run the node. If the output is not showing 0 for failed test, then you need to scroll through the testing output and find what test failed (It will be red instead of green). If this is a system compatibility test, then you will need to fix the system. If this is a core test that has failed, then you need to possibly rebuild, or contact us with your OS version, and we can look into it.


## Running X-CASH Proof of Stake - Website Server

Then you will need to run the xcash_proof_of_stake_website_server.
You will need to copy the xcash_proof_of_stake_website_server to the html folder of your web server.

Refer to the installation instructions at [X-CASH Proof of Stake - Website: NGINX Setup](https://github.com/X-CASH-official/XCASH_proof_of_stake_website#nginx-setup)

Navigate to the folder that contains the binary, then run  
`./xcash_proof_of_stake_website_server --fee FEE -- minimum_amount MINIMUM_AMOUNT`

Replace FEE a fee (you canuse up to 6 decimal places)
Replace MINIMUM_AMOUNT with the minimum amount a public address must have before receiving a payment

Note: Payments are sent out daily at UTC 00:00

We suggest you use the screen command to run the program in the background, this way you can still maintenance the server. To do this run  
`screen -dmS xcash_proof_of_stake_website_server ./xcash_proof_of_stake_website_server --fee FEE -- minimum_amount MINIMUM_AMOUNT`

To bring the screen from the bacground process to the active process run  
`screen -x NAME_OF_BACKGROUNDS_SCREEN`

Where NAME_OF_BACKGROUNDS_SCREEN would be xcash_proof_of_stake_consensus_node in the above example.

To exit a screen if it is the active process, you can press Control + C. To exit a screen that is a background process you can run  
`screen -XS NAME_OF_BACKGROUNDS_SCREEN quit`