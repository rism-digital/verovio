<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="svg">
    <xsl:output encoding="UTF-8" indent="yes" method="xml" omit-xml-declaration="yes"/>
    <xsl:variable name="supported">
        <supported>

            <!-- Clefs (U+E050 ‒ U+E07F) -->
            <glyph glyph-code="E050" smufl-name="gClef"/>
            <glyph glyph-code="E051" smufl-name="gClef15mb"/>
            <glyph glyph-code="E052" smufl-name="gClef8vb"/>
            <glyph glyph-code="E053" smufl-name="gClef8va"/>
            <glyph glyph-code="E054" smufl-name="gClef15ma"/>

            <glyph glyph-code="E05C" smufl-name="cClef"/>
            <glyph glyph-code="E05D" smufl-name="cClef8vb"/>

            <glyph glyph-code="E071" smufl-name="fClef"/>
            <glyph glyph-code="E072" smufl-name="fClef15mb"/>
            <glyph glyph-code="E073" smufl-name="fClef8vb"/>
            <glyph glyph-code="E074" smufl-name="fClef8va"/>
            <glyph glyph-code="E075" smufl-name="fClef8vb"/>

            <!-- Time signatures (U+E080 ‒ U+E09F) -->
            <glyph glyph-code="E080" smufl-name="timeSig0"/>
            <glyph glyph-code="E081" smufl-name="timeSig1"/>
            <glyph glyph-code="E082" smufl-name="timeSig2"/>
            <glyph glyph-code="E083" smufl-name="timeSig3"/>
            <glyph glyph-code="E084" smufl-name="timeSig4"/>
            <glyph glyph-code="E085" smufl-name="timeSig5"/>
            <glyph glyph-code="E086" smufl-name="timeSig6"/>
            <glyph glyph-code="E087" smufl-name="timeSig7"/>
            <glyph glyph-code="E088" smufl-name="timeSig8"/>
            <glyph glyph-code="E089" smufl-name="timeSig9"/>

            <!-- Noteheads (U+E0A0 ‒ U+E0FF) -->

            <glyph glyph-code="E0A2" smufl-name="noteheadWhole"/>
            <glyph glyph-code="E0A3" smufl-name="noteheadHalf"/>
            <glyph glyph-code="E0A4" smufl-name="noteheadBlack"/>

            <glyph glyph-code="E0FA" smufl-name="noteheadWholeFilled"/>

            <!-- Medieval and Renaissance clefs (U+E900 ‒ U+E90F) -->

            <!-- Medieval and Renaissance noteheads and stems (U+E930 ‒ U+E94F) -->

            <!-- Medieval and Renaissance accidentals (U+E9E0 ‒ U+E9EF) -->

        </supported>
    </xsl:variable>

    <xsl:template match="/" exclude-result-prefixes="svg xlink">
        <xsl:apply-templates select="//svg:glyph">
            <xsl:with-param name="fontName" select="//svg:font-face/@font-family"/>
            <xsl:with-param name="unitsPerEm" select="//svg:font-face/@units-per-em"/>
        </xsl:apply-templates>
    </xsl:template>

    <xsl:template match="svg:glyph" exclude-result-prefixes="xlink">
        <xsl:param name="fontName"/>
        <xsl:param name="unitsPerEm"/>
        <xsl:variable name="thisGlyph" select="@glyph-name"/>
        <xsl:if test="$supported/supported/glyph[concat('uni', @glyph-code)=$thisGlyph]">
            <xsl:variable name="glyphCode" select="substring-after(@glyph-name, 'uni')"/>
            <xsl:variable name="smuflName" select="$supported/supported/glyph[@glyph-code=$glyphCode]/@smufl-name"/>
            <xsl:result-document href="../data/{$fontName}/{$glyphCode}-{$smuflName}.xml">
                <symbol id="{$glyphCode}" viewBox="0 0 {$unitsPerEm} {$unitsPerEm}" overflow="inherit">
                    <g transform="scale(1,-1)">
                        <path>
                            <xsl:copy-of select="@d"/>
                        </path>
                    </g>
                </symbol>
            </xsl:result-document>
        </xsl:if>
    </xsl:template>

</xsl:stylesheet>
