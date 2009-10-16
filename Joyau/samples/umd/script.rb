# "Kinda" inspired of a sample found on sceners.org :
# http://sceners.org/?itemid=308

require 'joyau/old'

initLib

if !Umd.present?
  puts "No UMD present, please insert one"
  Umd.waitState Umd::PRESENT
  puts "Disc inserted"
end

Umd.mount
puts "Device mounted"

puts "Please remove that UMD"
Umd.waitState Umd::NOT_PRESENT

puts "Now, put another UMD."
Umd.permitReplace
Umd.waitState Umd::PRESENT
puts "disc inserted"

Umd.mount
puts "device mounted"

Umd.umount
puts "device unmounted. Please remove disc"

Umd.waitStat Umd::NOT_PRESENT

stopLib
