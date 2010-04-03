class SpriteTest < JoyauTest
  def setup
    Joyau.clearImages
  end

  def teardown
    Joyau.clearImages
  end

  def test_unexisting_file
    begin
      Joyau::Sprite.new("doesnt_exist.png")
    rescue Exception => e
      assert_equal RuntimeError, e.class
    end
  end

  def test_display
    sprite = Joyau::Sprite.new("res/image.png")
    
    Joyau::Pad.update
    Joyau.draw do
      Joyau::Pad.update
      sprite.draw
    end until Joyau::Pad.pressed? Joyau::Pad::START

    Joyau.draw { Joyau.clearScreen }
  end
  
  def test_auto_tile
    sprite = Joyau::Sprite.new("res/bg.png")
    old_width, old_height = sprite.w, sprite.h

    sprite.setAnim(4, 4)
    
    assert_equal(old_width  / 4, sprite.w)
    assert_equal(old_height / 4, sprite.h)

    Joyau::Pad.update
    Joyau.draw do
      Joyau::Pad.update
      sprite.draw
    end until Joyau::Pad.pressed? Joyau::Pad::START

    Joyau.draw { Joyau.clearScreen }
  end

  def test_manual_tile
    sprite = Joyau::Sprite.new("res/bg.png")
    sprite.setTile(10, 10, 30, 30)
    
    assert_equal 30, sprite.w
    assert_equal 30, sprite.h

    Joyau::Pad.update
    Joyau.draw do
      Joyau::Pad.update
      sprite.draw
    end until Joyau::Pad.pressed? Joyau::Pad::START
    
    Joyau.draw { Joyau.clearScreen }
  end
end
