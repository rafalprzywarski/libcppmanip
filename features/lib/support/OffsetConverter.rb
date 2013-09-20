require 'replacements'

class OffsetConverter
  def initialize text
    @text = text
  end
  def getLocationFromOffset offset
    SourceLocation.row_col 0, offset
  end
end