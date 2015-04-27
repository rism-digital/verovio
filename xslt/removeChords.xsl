<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:mei="http://www.music-encoding.org/ns/mei" xmlns:xd="http://www.oxygenxml.com/ns/doc/xsl" exclude-result-prefixes="xs xd" version="2.0">
    <xd:doc scope="stylesheet">
        <xd:desc>
            <xd:p>
                <xd:b>Created on:</xd:b> Sep 2, 2014</xd:p>
            <xd:p>
                <xd:b>Author:</xd:b> Johannes Kepper</xd:p>
            <xd:p/>
        </xd:desc>
    </xd:doc>
    <xsl:output method="xml" indent="yes"/>
    <xsl:template match="/">
        <xsl:apply-templates/>
    </xsl:template>
    <xsl:template match="mei:chord">
        <xsl:element name="rest" namespace="http://www.music-encoding.org/ns/mei">
            <xsl:attribute name="dur" select="./@dur"></xsl:attribute>
        </xsl:element>
    </xsl:template>
    
    <!--
    <xsl:template match="mei:chord/mei:note[1]">
        <xsl:copy-of select="."/>
    </xsl:template>
    
    <xsl:template match="mei:chord/mei:note[position > 1]">
    </xsl:template>
    -->

    <xsl:template match="node() | @*">
        <xsl:copy>
            <xsl:apply-templates select="node() | @*"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>