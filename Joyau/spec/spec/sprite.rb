require 'joyau_helper'
require 'spec/drawable.rb'

describe Joyau::Sprite do
  before :all do
    Joyau.clearImages
  end

  before :each do
    @drawable = Joyau::Sprite.new("res/bg.png")
  end

  it_should_behave_like "all drawables"

  it "should raise runtime error when loading unexisting files" do
    lambda = Proc.new do
      Joyau::Sprite.new("doesnt_exist.png")
    end

    lambda.should raise_error(RuntimeError)
  end

  it "should divide its size with auto tilling" do
    sprite = @drawable
    old_width, old_height = sprite.w, sprite.h

    sprite.setAnim(4, 4)
    
    sprite.w.should == (old_width / 4)
    sprite.h.should == (old_height / 4)
  end

  it "should change its size through manual tiles" do
    sprite = @drawable
    sprite.setTile(10, 10, 30, 30)

    sprite.w.should == 30
    sprite.h.should == 30
  end

  after :all do
    Joyau.clearImages
  end
end
