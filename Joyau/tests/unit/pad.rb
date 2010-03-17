class PadTest < JoyauTest
  def test_trigger
    Joyau::Pad.update
    Joyau::Pad.update until Joyau::Pad.pressed? Joyau::Pad::CROSS
    Joyau::Pad.update
    
    assert !(Joyau::Pad.pressed? Joyau::Pad::CROSS)
    assert(Joyau::Pad.held? Joyau::Pad::CROSS or
           Joyau::Pad.released? Joyau::Pad::CROSS)
  end
end
