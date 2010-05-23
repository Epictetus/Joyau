require 'net/ftp'

Joyau::Wlan.init

# Notice I'm using the second access point. You might want to use the first one.
# (in a true programme, you'd want to let the user select an acces point ;) 
# The timeout has been set arbitrary.
Joyau::Wlan.connect(2, 60)

# Now, this program looks pretty much like any other using Net:FTP :

ftp = Net::FTP.new
ftp.connect("ftp.ruby-lang.org", 21) # Ruby's FTP

ftp.login("anonymous", "")

ftp.getbinaryfile("pub/ruby/1.8/ruby-1.8.7-p248.tar.bz2", 
                  "ruby-1.8.7-p248.tar.bz2")
ftp.close


Joyau::Wlan.disconnect
Joyau::Wlan.stop
