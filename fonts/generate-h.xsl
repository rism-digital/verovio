<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="svg">
    <xsl:output encoding="UTF-8" indent="yes" method="text" omit-xml-declaration="yes"/>

    <xsl:template match="/" exclude-result-prefixes="svg xlink">
        
        <xsl:variable name="input-text" select="unparsed-text('smufl.h.inc', 'iso-8859-1')"/>
        <xsl:value-of select="$input-text"/>
        
        <!-- root of the bounding box svg file -->
        <xsl:text>enum {</xsl:text>
        <xsl:text>&#10;</xsl:text>
        <xsl:apply-templates select="//glyph">
        </xsl:apply-templates>
        <xsl:text>};</xsl:text>
        
        <xsl:text>&#10;&#10;/** The number of glyphs for verification **/</xsl:text>
        <xsl:text>&#10;#define SMUFL_COUNT </xsl:text>
        <xsl:value-of select="count(//glyph)"/>
        <xsl:text>&#10;</xsl:text>
        
        <!-- end of vrv namespace and of #ifdef -->
        <xsl:text>&#10;} // vrv namespace&#10;&#10;#endif&#10;</xsl:text>
        
    </xsl:template>

    <xsl:template match="glyph">
        <xsl:text>    SMUFL_</xsl:text>
        <xsl:value-of select="@glyph-code"/>_<xsl:value-of select="@smufl-name"/>
        <xsl:text disable-output-escaping="yes"> = 0x</xsl:text><xsl:value-of select="@glyph-code"/><xsl:text>,&#10;</xsl:text>
    </xsl:template>

</xsl:stylesheet>
