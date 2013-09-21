require 'OffsetConverter'

def offsetRangeFromPhrases startPhrase, endPhrase, source
  startIndex = source.index(startPhrase)
  endIndex = source.index(endPhrase)
  startIndex.should_not be_nil, "\"#{startPhrase}\" not found in the source code"
  endIndex.should_not be_nil, "\"#{endPhrase}\" not found in the source code"
  startIndex.should be <= endIndex, "\"#{startPhrase}\" must be before or equal to \"#{endPhrase}\""
  endIndex += endPhrase.length
  return startIndex, endIndex
end

def rangeFromPhrases startPhrase, endPhrase, source
  startOff, endOff = offsetRangeFromPhrases startPhrase, endPhrase, source
  conv = OffsetConverter.new source
  return conv.getLocationFromOffset(startOff), conv.getLocationFromOffset(endOff)
end
