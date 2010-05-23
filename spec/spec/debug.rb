require 'joyau_helper'

describe Joyau::Debug do
  before :each do
    Joyau::Debug.clear
    Joyau::Debug.init
  end

  it "should be movable" do
    Joyau::Debug.x = 9
    Joyau::Debug.y = 2
    
    Joyau::Debug.should be_at(9, 2)
  end

  it "should reset the cursor's position when re-inited" do
    Joyau::Debug.x = 20
    Joyau::Debug.y = 5
    
    Joyau::Debug.clear
    Joyau::Debug.init
    
    Joyau::Debug.should be_at(0, 0)
  end

  it "should change the cursor's position when changing line" do
    Joyau::Debug.puts
    Joyau::Debug.should be_at(0, 1)
  end
end
