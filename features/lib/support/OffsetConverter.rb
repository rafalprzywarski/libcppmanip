require 'replacements'

class OffsetConverter
  def initialize text
    @text = text
  end
  def getLocationFromOffset offset
    SourceLocation.row_col calc_row(offset), calc_column(offset)
  end
  private
  def calc_column offset
    last_eol = @text.rindex("\n", offset - 1) if offset > 0
    last_eol ? offset - last_eol - 1 : offset
  end
  def calc_row offset
    @text.slice(0..offset - 1).count("\n")
  end
end