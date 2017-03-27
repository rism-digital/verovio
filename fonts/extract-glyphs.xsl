<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="svg">
    <xsl:output encoding="UTF-8" indent="yes" method="xml" omit-xml-declaration="yes"/>

    <xsl:include href="supported.xsl"/>

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
        <xsl:variable name="thisGlyph" select="@glyph-name"/>
        <xsl:if test="$supported/*/glyph[concat('uni', @glyph-code)=$thisGlyph]">
            <xsl:variable name="glyphCode" select="substring-after(@glyph-name, 'uni')"/>
            <xsl:variable name="smuflName" select="$supported/*/glyph[@glyph-code=$glyphCode]/@smufl-name"/>

            <!-- redirect to a file for each glyph -->
            <xsl:result-document href="../data/{$fontName}/{$glyphCode}-{$smuflName}.xml">
                <symbol id="{$glyphCode}" viewBox="0 0 {$unitsPerEm} {$unitsPerEm}" overflow="inherit">
                    <path>
                        <xsl:attribute name="transform">
                            <xsl:text>scale(1,-1)</xsl:text>
                        </xsl:attribute>
                        <xsl:copy-of select="@d"/>
                    </path>
                </symbol>
            </xsl:result-document>

            <!-- write the glyph to the bounding box svg file -->
            <path xmlns="http://www.w3.org/2000/svg" name="{$smuflName}" id="{$glyphCode}" horiz-adv-x="{@horiz-adv-x}">
                <xsl:attribute name="transform">
                    <xsl:text>scale(1,-1)</xsl:text>
                </xsl:attribute>
                <xsl:copy-of select="@d"/>
            </path>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
