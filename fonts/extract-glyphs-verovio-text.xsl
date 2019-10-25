<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="svg">
    <xsl:output encoding="UTF-8" indent="yes" method="xml" omit-xml-declaration="yes"/>

    <xsl:template match="/" exclude-result-prefixes="svg xlink">
        <!-- root of the bounding box svg file -->
        <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
            <xsl:attribute name="font-family" select="//svg:font-face/@font-family"/>
            <xsl:apply-templates select="//svg:font-face"/>
            <xsl:apply-templates select="//svg:glyph">
                <xsl:with-param name="fontName" select="//svg:font-face/@font-family"/>
                <xsl:with-param name="unitsPerEm" select="//svg:font-face/@units-per-em"/>
            </xsl:apply-templates>
        </svg>
    </xsl:template>


    <xsl:template match="svg:font-face" exclude-result-prefixes="svg xlink" >
        <font-face xmlns="http://www.w3.org/2000/svg">
            <xsl:copy-of select="@units-per-em"/>
        </font-face>
    </xsl:template>

    <xsl:template match="svg:glyph" exclude-result-prefixes="xlink svg">
        <xsl:param name="fontName"/>
        <xsl:param name="unitsPerEm"/>
        <xsl:variable name="thisGlyph" select="@unicode"/>
        <xsl:variable name="horiz-adv-x" select="@horiz-adv-x"/>
        
        <xsl:if test="@glyph-name and @glyph-name != '.notdef'">
        	<xsl:variable name="glyphCode" select="substring-after(@glyph-name, 'uni')"/>
            <!-- write the glyph to the bounding box svg file -->
            <path xmlns="http://www.w3.org/2000/svg" transform="scale(1.0,-1.0)" id="{$glyphCode}" horiz-adv-x="{$horiz-adv-x}">
                <xsl:copy-of select="@d"/>
            </path>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
