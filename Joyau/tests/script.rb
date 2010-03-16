require 'joyau'
require 'test/unit'

use_wlan      = false
connection_id = 2

Joyau.init(:lib, :gfx, :intrafont, :audio, :osl_audio, :usb)

if use_wlan
  Joyau.init(:wlan)
  Joyau::Wlan.connect(connection_id, 30)
end

JoyauTest = Test::Unit::TestCase
Dir["unit/*.rb"].each { |file| require file }
Test::Unit::AutoRunner.run

Joyau::Wlan.disconnect if use_wlan
Joyau.stop
