#[osx\_energy\_prefs]

A Max/MSP external and shell scripts to modify energy preferences on OS X.

Built against max-sdk-7.1.0 on OS X 10.11. No idea about backwards compat. 

Written as part of Robin Fox's *Giant Theremin* project in May 2016.

##Setup

    git clone https://github.com/Cycling74/max-sdk.git
    cd max-sdk-${VERSION}/source
	git clone https://github.com/tbrodel/osx_open.git

After building in Xcode the external will be found in
    max-sdk-${VERSION}/externals

The filepath to the scripts is hardcoded to the external, if you don't want to 
customise it in the code then copy them over this way:
	cd osx_open
	mkdir -p /Users/Shared/giant_theremin/scripts
	rsync -a scripts/ /Users/Shared/giant_theremin/scripts

The external relies upon a <pre><code>cron(8)</code></pre> job run as root. 
<pre><code>gt\_energy.cron</code></pre> is an example, you need to install 
something similar by running:
    sudo crontab -e
