# Prints "Hello world!" on the screen. No assertions here, as I couldn't
# find something to check.
#
# When running this test, you should see it though.

class HelloTest < JoyauTest
  def test_hello_world
    Joyau::Pad.update
    Joyau.draw do
      Joyau::Pad.update
      Joyau.drawText(0, 0, "Hello world!")
    end until Joyau::Pad.pressed? Joyau::Pad::START
  
    Joyau.draw { Joyau.clearScreen }
  end
end
