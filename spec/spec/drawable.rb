require 'joyau_helper'

shared_examples_for "all drawables" do
  it "should not have moved after being drawn" do
    @drawable.move(100, 100)
    Joyau.draw { @drawable.draw }
    
    @drawable.should_not have_moved
  end

  it "should not have moved after having cancelled moves" do
    @drawable.move(100, 100)
    @drawable.cancelMove

    @drawable.should_not have_moved
  end

  it "should uncount back moves" do
    @drawable.move(100, 100)
    @drawable.move(-10, -40)

    @drawable.should be_at(90, 60)
    @drawable.should have_moved_of(90, 60)
  end

  it "should not have moved when created" do
    @drawable.should_not have_moved
  end
end

describe Joyau::Drawable do
  before :each do
    @drawable = Joyau::Drawable.new
  end

  it_should_behave_like "all drawables"
end
