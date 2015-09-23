<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  
    xmlns:mei="http://www.music-encoding.org/ns/mei"
    version="1.0" exclude-result-prefixes="mei">
    
    <xsl:output omit-xml-declaration="yes" encoding="UTF-8" indent="yes"/>
    <xsl:strip-space elements="*"/>
    
    <xsl:template match="mei:*[@label='feature-example']">  
        <xsl:text>&#xa;</xsl:text>
        <xsl:element name="{local-name()}">
            <xsl:copy-of select="@*[not(name()='label')]"/>
            <xsl:apply-templates mode="copy-no-namespaces"></xsl:apply-templates>    
        </xsl:element>
    </xsl:template>
    
    <xsl:template match="*" mode="copy-no-namespaces">
        <xsl:element name="{local-name()}">
            <xsl:copy-of select="@*"/>
            <xsl:apply-templates select="node()" mode="copy-no-namespaces"/>
        </xsl:element>
    </xsl:template>
    
    <xsl:template match="comment()| processing-instruction()" mode="copy-no-namespaces">
        <xsl:copy/>
    </xsl:template>
   
    <xsl:template match="text()"/>
</xsl:stylesheet>
