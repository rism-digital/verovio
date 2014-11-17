<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="svg">
    <xsl:output encoding="UTF-8" indent="yes" method="text" omit-xml-declaration="yes"/>

    <xsl:template match="/" exclude-result-prefixes="svg xlink">
        
        <xsl:variable name="input-text" select="unparsed-text('test.txt', 'iso-8859-1')"/>
        <xsl:value-of select="$input-text"/>
        
        <!-- root of the bounding box svg file -->
        <xsl:apply-templates select="//glyph">
        </xsl:apply-templates>
        <xsl:text>&#10;</xsl:text>
        
    </xsl:template>

    <xsl:template match="glyph">
        
        <xsl:variable name="glyphName">
            <xsl:value-of select="@glyph-code"/>_<xsl:value-of select="@smufl-name"/>
        </xsl:variable>
        
        <xsl:text>m_glyphs[SMUFL_</xsl:text><xsl:value-of select="$glyphName"/><xsl:text>]</xsl:text>
        <xsl:text> = Glyph("</xsl:text><xsl:value-of select="$glyphName"/><xsl:text>");&#10;</xsl:text>
    </xsl:template>

</xsl:stylesheet>
