require 'rspec'
require 'OffsetConverter'

describe :OffsetCoverter do
  it "should return offset in the first line" do
    conv = OffsetConverter.new("xxxxx\nx\nxxxxxxxxx")
    loc = conv.getLocationFromOffset(5)
    loc.col.should eq(5)
    loc.row.should eq(0)
    conv.getLocationFromOffset(0).col.should eq(0)
  end
  it "should return offset from the last eol as col" do
    conv = OffsetConverter.new("x\nx\nxxxxx\n");
    conv.getLocationFromOffset(3).col.should eq(1)
    conv.getLocationFromOffset(4).col.should eq(0)
    conv.getLocationFromOffset(5).col.should eq(1)
  end
end
