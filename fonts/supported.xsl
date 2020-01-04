<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:svg="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="svg">
    <xsl:variable name="supported">
        <staffBracketsAndDividers>
            <description>Staff brackets and dividers</description>
            <!--<glyph glyph-code="E000" smufl-name="brace"/>-->
            <!--<glyph glyph-code="E001" smufl-name="reversedBrace"/>-->
            <!--<glyph glyph-code="E002" smufl-name="bracket"/>-->
            <glyph glyph-code="E003" smufl-name="bracketTop"/>
            <glyph glyph-code="E004" smufl-name="bracketBottom"/>
            <!--<glyph glyph-code="E005" smufl-name="reversedBracketTop"/>-->
            <!--<glyph glyph-code="E006" smufl-name="reversedBracketBottom"/>-->
            <!--<glyph glyph-code="E007" smufl-name="systemDivider"/>-->
            <!--<glyph glyph-code="E008" smufl-name="systemDividerLong"/>-->
            <!--<glyph glyph-code="E009" smufl-name="systemDividerExtraLong"/>-->
            <!--<glyph glyph-code="E00A" smufl-name="splitBarDivider"/>-->
            <!--<glyph glyph-code="E00B" smufl-name="staffDivideArrowDown"/>-->
            <!--<glyph glyph-code="E00C" smufl-name="staffDivideArrowUp"/>-->
            <!--<glyph glyph-code="E00D" smufl-name="staffDivideArrowUpDown"/>-->
            <range_end>U+E00F</range_end>
            <range_start>U+E000</range_start>
        </staffBracketsAndDividers>
        <staves>
            <description>Staves</description>
            <!--<glyph glyph-code="E010" smufl-name="staff1Line"/>-->
            <!--<glyph glyph-code="E011" smufl-name="staff2Lines"/>-->
            <!--<glyph glyph-code="E012" smufl-name="staff3Lines"/>-->
            <!--<glyph glyph-code="E013" smufl-name="staff4Lines"/>-->
            <!--<glyph glyph-code="E014" smufl-name="staff5Lines"/>-->
            <!--<glyph glyph-code="E015" smufl-name="staff6Lines"/>-->
            <!--<glyph glyph-code="E016" smufl-name="staff1LineWide"/>-->
            <!--<glyph glyph-code="E017" smufl-name="staff2LinesWide"/>-->
            <!--<glyph glyph-code="E018" smufl-name="staff3LinesWide"/>-->
            <!--<glyph glyph-code="E019" smufl-name="staff4LinesWide"/>-->
            <!--<glyph glyph-code="E01A" smufl-name="staff5LinesWide"/>-->
            <!--<glyph glyph-code="E01B" smufl-name="staff6LinesWide"/>-->
            <!--<glyph glyph-code="E01C" smufl-name="staff1LineNarrow"/>-->
            <!--<glyph glyph-code="E01D" smufl-name="staff2LinesNarrow"/>-->
            <!--<glyph glyph-code="E01E" smufl-name="staff3LinesNarrow"/>-->
            <!--<glyph glyph-code="E01F" smufl-name="staff4LinesNarrow"/>-->
            <!--<glyph glyph-code="E020" smufl-name="staff5LinesNarrow"/>-->
            <!--<glyph glyph-code="E021" smufl-name="staff6LinesNarrow"/>-->
            <!--<glyph glyph-code="E022" smufl-name="legerLine"/>-->
            <!--<glyph glyph-code="E023" smufl-name="legerLineWide"/>-->
            <!--<glyph glyph-code="E024" smufl-name="legerLineNarrow"/>-->
            <range_end>U+E02F</range_end>
            <range_start>U+E010</range_start>
        </staves>
        <barlines>
            <description>Barlines</description>
            <!--<glyph glyph-code="E030" smufl-name="barlineSingle"/>-->
            <!--<glyph glyph-code="E031" smufl-name="barlineDouble"/>-->
            <!--<glyph glyph-code="E032" smufl-name="barlineFinal"/>-->
            <!--<glyph glyph-code="E033" smufl-name="barlineReverseFinal"/>-->
            <!--<glyph glyph-code="E034" smufl-name="barlineHeavy"/>-->
            <!--<glyph glyph-code="E035" smufl-name="barlineHeavyHeavy"/>-->
            <!--<glyph glyph-code="E036" smufl-name="barlineDashed"/>-->
            <!--<glyph glyph-code="E037" smufl-name="barlineDotted"/>-->
            <!--<glyph glyph-code="E038" smufl-name="barlineShort"/>-->
            <!--<glyph glyph-code="E039" smufl-name="barlineTick"/>-->
            <range_end>U+E03F</range_end>
            <range_start>U+E030</range_start>
        </barlines>
        <repeats>
            <description>Repeats</description>
            <!--<glyph glyph-code="E040" smufl-name="repeatLeft"/>-->
            <!--<glyph glyph-code="E041" smufl-name="repeatRight"/>-->
            <!--<glyph glyph-code="E042" smufl-name="repeatRightLeft"/>-->
            <!--<glyph glyph-code="E043" smufl-name="repeatDots"/>-->
            <!--<glyph glyph-code="E044" smufl-name="repeatDot"/>-->
            <glyph glyph-code="E045" smufl-name="dalSegno"/>
            <glyph glyph-code="E046" smufl-name="daCapo"/>
            <glyph glyph-code="E047" smufl-name="segno"/>
            <glyph glyph-code="E048" smufl-name="coda"/>
            <!--<glyph glyph-code="E049" smufl-name="codaSquare"/>-->
            <!--<glyph glyph-code="E04A" smufl-name="segnoSerpent1"/>-->
            <glyph glyph-code="E04B" smufl-name="segnoSerpent2"/>
            <!--<glyph glyph-code="E04C" smufl-name="leftRepeatSmall"/>-->
            <!--<glyph glyph-code="E04D" smufl-name="rightRepeatSmall"/>-->
            <range_end>U+E04F</range_end>
            <range_start>U+E040</range_start>
        </repeats>
        <clefs>
            <description>Clefs</description>
            <glyph glyph-code="E050" smufl-name="gClef"/>
            <glyph glyph-code="E051" smufl-name="gClef15mb"/>
            <glyph glyph-code="E052" smufl-name="gClef8vb"/>
            <glyph glyph-code="E053" smufl-name="gClef8va"/>
            <glyph glyph-code="E054" smufl-name="gClef15ma"/>
            <!--<glyph glyph-code="E055" smufl-name="gClef8vbOld"/>-->
            <!--<glyph glyph-code="E056" smufl-name="gClef8vbCClef"/>-->
            <!--<glyph glyph-code="E057" smufl-name="gClef8vbParens"/>-->
            <!--<glyph glyph-code="E058" smufl-name="gClefLigatedNumberBelow"/>-->
            <!--<glyph glyph-code="E059" smufl-name="gClefLigatedNumberAbove"/>-->
            <!--<glyph glyph-code="E05A" smufl-name="gClefArrowUp"/>-->
            <!--<glyph glyph-code="E05B" smufl-name="gClefArrowDown"/>-->
            <glyph glyph-code="E05C" smufl-name="cClef"/>
            <glyph glyph-code="E05D" smufl-name="cClef8vb"/>
            <!--<glyph glyph-code="E05E" smufl-name="cClefArrowUp"/>-->
            <!--<glyph glyph-code="E05F" smufl-name="cClefArrowDown"/>-->
            <!--<glyph glyph-code="E060" smufl-name="cClefSquare"/>-->
            <!--<glyph glyph-code="E061" smufl-name="cClefCombining"/>-->
            <glyph glyph-code="E062" smufl-name="fClef"/>-->
            <glyph glyph-code="E063" smufl-name="fClef15mb"/>
            <glyph glyph-code="E064" smufl-name="fClef8vb"/>
            <glyph glyph-code="E065" smufl-name="fClef8va"/>
            <glyph glyph-code="E066" smufl-name="fClef15ma"/>
            <!--<glyph glyph-code="E067" smufl-name="fClefArrowUp"/>-->
            <!--<glyph glyph-code="E068" smufl-name="fClefArrowDown"/>-->
            <glyph glyph-code="E069" smufl-name="unpitchedPercussionClef1"/>
            <glyph glyph-code="E06A" smufl-name="unpitchedPercussionClef2"/>
            <!--<glyph glyph-code="E06B" smufl-name="semipitchedPercussionClef1"/>-->
            <!--<glyph glyph-code="E06C" smufl-name="semipitchedPercussionClef2"/>-->
            <!--<glyph glyph-code="E06F" smufl-name="cClefTriangular"/>-->
            <!--<glyph glyph-code="E070" smufl-name="fClefTriangular"/>-->
            <!--<glyph glyph-code="E071" smufl-name="cClefTriangularToFClef"/>-->
            <!--<glyph glyph-code="E072" smufl-name="fClefTriangularToCClef"/>-->
            <!--<glyph glyph-code="E073" smufl-name="gClefReversed"/>-->
            <!--<glyph glyph-code="E074" smufl-name="gClefTurned"/>-->
            <!--<glyph glyph-code="E075" smufl-name="cClefReversed"/>-->
            <!--<glyph glyph-code="E076" smufl-name="fClefReversed"/>-->
            <!--<glyph glyph-code="E077" smufl-name="fClefTurned"/>-->
            <!--<glyph glyph-code="E078" smufl-name="bridgeClef"/>-->
            <!--<glyph glyph-code="E079" smufl-name="accdnDiatonicClef"/>-->
            <!--<glyph glyph-code="E07A" smufl-name="gClefChange"/>-->
            <!--<glyph glyph-code="E07B" smufl-name="cClefChange"/>-->
            <!--<glyph glyph-code="E07C" smufl-name="fClefChange"/>-->
            <!--<glyph glyph-code="E07D" smufl-name="clef8"/>-->
            <!--<glyph glyph-code="E07E" smufl-name="clef15"/>-->
            <!--<glyph glyph-code="E07F" smufl-name="clefChangeCombining"/>-->
            <range_end>U+E07F</range_end>
            <range_start>U+E050</range_start>
        </clefs>
        <timeSignatures>
            <description>Time signatures</description>
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
            <glyph glyph-code="E08A" smufl-name="timeSigCommon"/>
            <glyph glyph-code="E08B" smufl-name="timeSigCutCommon"/>
            <glyph glyph-code="E08C" smufl-name="timeSigPlus"/>
            <glyph glyph-code="E08D" smufl-name="timeSigPlusSmall"/>
            <!--<glyph glyph-code="E08E" smufl-name="timeSigFractionalSlash"/>-->
            <!--<glyph glyph-code="E08F" smufl-name="timeSigEquals"/>-->
            <!--<glyph glyph-code="E090" smufl-name="timeSigMinus"/>-->
            <!--<glyph glyph-code="E091" smufl-name="timeSigMultiply"/>-->
            <!--<glyph glyph-code="E092" smufl-name="timeSigParensLeftSmall"/>-->
            <!--<glyph glyph-code="E093" smufl-name="timeSigParensRightSmall"/>-->
            <!--<glyph glyph-code="E094" smufl-name="timeSigParensLeft"/>-->
            <!--<glyph glyph-code="E095" smufl-name="timeSigParensRight"/>-->
            <!--<glyph glyph-code="E096" smufl-name="timeSigComma"/>-->
            <!--<glyph glyph-code="E097" smufl-name="timeSigFractionQuarter"/>-->
            <!--<glyph glyph-code="E098" smufl-name="timeSigFractionHalf"/>-->
            <!--<glyph glyph-code="E099" smufl-name="timeSigFractionThreeQuarters"/>-->
            <!--<glyph glyph-code="E09A" smufl-name="timeSigFractionOneThird"/>-->
            <!--<glyph glyph-code="E09B" smufl-name="timeSigFractionTwoThirds"/>-->
            <!--<glyph glyph-code="E09C" smufl-name="timeSigX"/>-->
            <!--<glyph glyph-code="E09D" smufl-name="timeSigOpenPenderecki"/>-->
            <!--<glyph glyph-code="E09E" smufl-name="timeSigCombNumerator"/>-->
            <!--<glyph glyph-code="E09F" smufl-name="timeSigCombDenominator"/>-->
            <range_end>U+E09F</range_end>
            <range_start>U+E080</range_start>
        </timeSignatures>
        <noteheads>
            <description>Noteheads</description>
            <!--<glyph glyph-code="E0A0" smufl-name="noteheadDoubleWhole"/>-->
            <!--<glyph glyph-code="E0A1" smufl-name="noteheadDoubleWholeSquare"/>-->
            <glyph glyph-code="E0A2" smufl-name="noteheadWhole"/>
            <glyph glyph-code="E0A3" smufl-name="noteheadHalf"/>
            <glyph glyph-code="E0A4" smufl-name="noteheadBlack"/>
            <!--<glyph glyph-code="E0A5" smufl-name="noteheadNull"/>-->
            <!--<glyph glyph-code="E0A6" smufl-name="noteheadXDoubleWhole"/>-->
            <!--<glyph glyph-code="E0A7" smufl-name="noteheadXWhole"/>-->
            <!--<glyph glyph-code="E0A8" smufl-name="noteheadXHalf"/>-->
            <!--<glyph glyph-code="E0A9" smufl-name="noteheadXBlack"/>-->
            <!--<glyph glyph-code="E0AA" smufl-name="noteheadXOrnate"/>-->
            <!--<glyph glyph-code="E0AB" smufl-name="noteheadXOrnateEllipse"/>-->
            <!--<glyph glyph-code="E0AC" smufl-name="noteheadPlusDoubleWhole"/>-->
            <!--<glyph glyph-code="E0AD" smufl-name="noteheadPlusWhole"/>-->
            <!--<glyph glyph-code="E0AE" smufl-name="noteheadPlusHalf"/>-->
            <!--<glyph glyph-code="E0AF" smufl-name="noteheadPlusBlack"/>-->
            <!--<glyph glyph-code="E0B0" smufl-name="noteheadCircleXDoubleWhole"/>-->
            <!--<glyph glyph-code="E0B1" smufl-name="noteheadCircleXWhole"/>-->
            <!--<glyph glyph-code="E0B2" smufl-name="noteheadCircleXHalf"/>-->
            <!--<glyph glyph-code="E0B3" smufl-name="noteheadCircleX"/>-->
            <!--<glyph glyph-code="E0B4" smufl-name="noteheadDoubleWholeWithX"/>-->
            <!--<glyph glyph-code="E0B5" smufl-name="noteheadWholeWithX"/>-->
            <!--<glyph glyph-code="E0B6" smufl-name="noteheadHalfWithX"/>-->
            <!--<glyph glyph-code="E0B7" smufl-name="noteheadVoidWithX"/>-->
            <!--<glyph glyph-code="E0B8" smufl-name="noteheadSquareWhite"/>-->
            <!--<glyph glyph-code="E0B9" smufl-name="noteheadSquareBlack"/>-->
            <!--<glyph glyph-code="E0BA" smufl-name="noteheadTriangleUpDoubleWhole"/>-->
            <!--<glyph glyph-code="E0BB" smufl-name="noteheadTriangleUpWhole"/>-->
            <!--<glyph glyph-code="E0BC" smufl-name="noteheadTriangleUpHalf"/>-->
            <!--<glyph glyph-code="E0BD" smufl-name="noteheadTriangleUpWhite"/>-->
            <!--<glyph glyph-code="E0BE" smufl-name="noteheadTriangleUpBlack"/>-->
            <!--<glyph glyph-code="E0BF" smufl-name="noteheadTriangleLeftWhite"/>-->
            <!--<glyph glyph-code="E0C0" smufl-name="noteheadTriangleLeftBlack"/>-->
            <!--<glyph glyph-code="E0C1" smufl-name="noteheadTriangleRightWhite"/>-->
            <!--<glyph glyph-code="E0C2" smufl-name="noteheadTriangleRightBlack"/>-->
            <!--<glyph glyph-code="E0C3" smufl-name="noteheadTriangleDownDoubleWhole"/>-->
            <!--<glyph glyph-code="E0C4" smufl-name="noteheadTriangleDownWhole"/>-->
            <!--<glyph glyph-code="E0C5" smufl-name="noteheadTriangleDownHalf"/>-->
            <!--<glyph glyph-code="E0C6" smufl-name="noteheadTriangleDownWhite"/>-->
            <!--<glyph glyph-code="E0C7" smufl-name="noteheadTriangleDownBlack"/>-->
            <!--<glyph glyph-code="E0C8" smufl-name="noteheadTriangleUpRightWhite"/>-->
            <!--<glyph glyph-code="E0C9" smufl-name="noteheadTriangleUpRightBlack"/>-->
            <!--<glyph glyph-code="E0CA" smufl-name="noteheadMoonWhite"/>-->
            <!--<glyph glyph-code="E0CB" smufl-name="noteheadMoonBlack"/>-->
            <!--<glyph glyph-code="E0CC" smufl-name="noteheadTriangleRoundDownWhite"/>-->
            <!--<glyph glyph-code="E0CD" smufl-name="noteheadTriangleRoundDownBlack"/>-->
            <!--<glyph glyph-code="E0CE" smufl-name="noteheadParenthesis"/>-->
            <!--<glyph glyph-code="E0CF" smufl-name="noteheadSlashedBlack1"/>-->
            <!--<glyph glyph-code="E0D0" smufl-name="noteheadSlashedBlack2"/>-->
            <!--<glyph glyph-code="E0D1" smufl-name="noteheadSlashedHalf1"/>-->
            <!--<glyph glyph-code="E0D2" smufl-name="noteheadSlashedHalf2"/>-->
            <!--<glyph glyph-code="E0D3" smufl-name="noteheadSlashedWhole1"/>-->
            <!--<glyph glyph-code="E0D4" smufl-name="noteheadSlashedWhole2"/>-->
            <!--<glyph glyph-code="E0D5" smufl-name="noteheadSlashedDoubleWhole1"/>-->
            <!--<glyph glyph-code="E0D6" smufl-name="noteheadSlashedDoubleWhole2"/>-->
            <!--<glyph glyph-code="E0D7" smufl-name="noteheadDiamondDoubleWhole"/>-->
            <!--<glyph glyph-code="E0D8" smufl-name="noteheadDiamondWhole"/>-->
            <!--<glyph glyph-code="E0D9" smufl-name="noteheadDiamondHalf"/>-->
            <!--<glyph glyph-code="E0DA" smufl-name="noteheadDiamondHalfWide"/>-->
            <!--<glyph glyph-code="E0DB" smufl-name="noteheadDiamondBlack"/>-->
            <!--<glyph glyph-code="E0DC" smufl-name="noteheadDiamondBlackWide"/>-->
            <!--<glyph glyph-code="E0DD" smufl-name="noteheadDiamondWhite"/>-->
            <!--<glyph glyph-code="E0DE" smufl-name="noteheadDiamondWhiteWide"/>-->
            <!--<glyph glyph-code="E0DF" smufl-name="noteheadDiamondDoubleWholeOld"/>-->
            <!--<glyph glyph-code="E0E0" smufl-name="noteheadDiamondWholeOld"/>-->
            <!--<glyph glyph-code="E0E1" smufl-name="noteheadDiamondHalfOld"/>-->
            <!--<glyph glyph-code="E0E2" smufl-name="noteheadDiamondBlackOld"/>-->
            <!--<glyph glyph-code="E0E3" smufl-name="noteheadDiamondHalfFilled"/>-->
            <!--<glyph glyph-code="E0E4" smufl-name="noteheadCircledBlack"/>-->
            <!--<glyph glyph-code="E0E5" smufl-name="noteheadCircledHalf"/>-->
            <!--<glyph glyph-code="E0E6" smufl-name="noteheadCircledWhole"/>-->
            <!--<glyph glyph-code="E0E7" smufl-name="noteheadCircledDoubleWhole"/>-->
            <!--<glyph glyph-code="E0E8" smufl-name="noteheadCircledBlackLarge"/>-->
            <!--<glyph glyph-code="E0E9" smufl-name="noteheadCircledHalfLarge"/>-->
            <!--<glyph glyph-code="E0EA" smufl-name="noteheadCircledWholeLarge"/>-->
            <!--<glyph glyph-code="E0EB" smufl-name="noteheadCircledDoubleWholeLarge"/>-->
            <!--<glyph glyph-code="E0EC" smufl-name="noteheadCircledXLarge"/>-->
            <!--<glyph glyph-code="E0ED" smufl-name="noteheadLargeArrowUpDoubleWhole"/>-->
            <!--<glyph glyph-code="E0EE" smufl-name="noteheadLargeArrowUpWhole"/>-->
            <!--<glyph glyph-code="E0EF" smufl-name="noteheadLargeArrowUpHalf"/>-->
            <!--<glyph glyph-code="E0F0" smufl-name="noteheadLargeArrowUpBlack"/>-->
            <!--<glyph glyph-code="E0F1" smufl-name="noteheadLargeArrowDownDoubleWhole"/>-->
            <!--<glyph glyph-code="E0F2" smufl-name="noteheadLargeArrowDownWhole"/>-->
            <!--<glyph glyph-code="E0F3" smufl-name="noteheadLargeArrowDownHalf"/>-->
            <!--<glyph glyph-code="E0F4" smufl-name="noteheadLargeArrowDownBlack"/>-->
            <!--<glyph glyph-code="E0F5" smufl-name="noteheadParenthesisLeft"/>-->
            <!--<glyph glyph-code="E0F6" smufl-name="noteheadParenthesisRight"/>-->
            <!--<glyph glyph-code="E0F7" smufl-name="noteheadCircleSlash"/>-->
            <!--<glyph glyph-code="E0F8" smufl-name="noteheadHeavyX"/>-->
            <!--<glyph glyph-code="E0F9" smufl-name="noteheadHeavyXHat"/>-->
            <glyph glyph-code="E0FA" smufl-name="noteheadWholeFilled"/>
            <!--<glyph glyph-code="E0FB" smufl-name="noteheadHalfFilled"/>-->
            <!--<glyph glyph-code="E0FC" smufl-name="noteheadDiamondOpen"/>-->
            <range_end>U+E0FF</range_end>
            <range_start>U+E0A0</range_start>
        </noteheads>
        <slashNoteheads>
            <description>Slash noteheads</description>
            <!--<glyph glyph-code="E100" smufl-name="noteheadSlashVerticalEnds"/>-->
            <glyph glyph-code="E101" smufl-name="noteheadSlashHorizontalEnds"/>
            <!--<glyph glyph-code="E102" smufl-name="noteheadSlashWhiteWhole"/>-->
            <!--<glyph glyph-code="E103" smufl-name="noteheadSlashWhiteHalf"/>-->
            <!--<glyph glyph-code="E104" smufl-name="noteheadSlashDiamondWhite"/>-->
            <!--<glyph glyph-code="E105" smufl-name="noteheadSlashVerticalEndsSmall"/>-->
            <!--<glyph glyph-code="E106" smufl-name="noteheadSlashX"/>-->
            <!--<glyph glyph-code="E107" smufl-name="noteheadSlashVerticalEndsMuted"/>-->
            <!--<glyph glyph-code="E108" smufl-name="noteheadSlashHorizontalEndsMuted"/>-->
            <!--<glyph glyph-code="E109" smufl-name="noteheadSlashWhiteMuted"/>-->
            <range_end>U+E10F</range_end>
            <range_start>U+E100</range_start>
        </slashNoteheads>
        <roundAndSquareNoteheads>
            <description>Round and square noteheads</description>
            <!--<glyph glyph-code="E110" smufl-name="noteheadRoundBlackLarge"/>-->
            <!--<glyph glyph-code="E111" smufl-name="noteheadRoundWhiteLarge"/>-->
            <!--<glyph glyph-code="E112" smufl-name="noteheadRoundWhiteWithDotLarge"/>-->
            <!--<glyph glyph-code="E113" smufl-name="noteheadRoundBlack"/>-->
            <!--<glyph glyph-code="E114" smufl-name="noteheadRoundWhite"/>-->
            <!--<glyph glyph-code="E115" smufl-name="noteheadRoundWhiteWithDot"/>-->
            <!--<glyph glyph-code="E116" smufl-name="noteheadRoundBlackSlashedLarge"/>-->
            <!--<glyph glyph-code="E117" smufl-name="noteheadRoundWhiteSlashedLarge"/>-->
            <!--<glyph glyph-code="E118" smufl-name="noteheadRoundBlackSlashed"/>-->
            <!--<glyph glyph-code="E119" smufl-name="noteheadRoundWhiteSlashed"/>-->
            <!--<glyph glyph-code="E11A" smufl-name="noteheadSquareBlackLarge"/>-->
            <!--<glyph glyph-code="E11B" smufl-name="noteheadSquareBlackWhite"/>-->
            <range_end>U+E11F</range_end>
            <range_start>U+E110</range_start>
        </roundAndSquareNoteheads>
        <noteClusters>
            <description>Note clusters</description>
            <!--<glyph glyph-code="E120" smufl-name="noteheadClusterSquareWhite"/>-->
            <!--<glyph glyph-code="E121" smufl-name="noteheadClusterSquareBlack"/>-->
            <!--<glyph glyph-code="E122" smufl-name="noteheadClusterRoundWhite"/>-->
            <!--<glyph glyph-code="E123" smufl-name="noteheadClusterRoundBlack"/>-->
            <!--<glyph glyph-code="E124" smufl-name="noteheadClusterDoubleWhole2nd"/>-->
            <!--<glyph glyph-code="E125" smufl-name="noteheadClusterWhole2nd"/>-->
            <!--<glyph glyph-code="E126" smufl-name="noteheadClusterHalf2nd"/>-->
            <!--<glyph glyph-code="E127" smufl-name="noteheadClusterQuarter2nd"/>-->
            <!--<glyph glyph-code="E128" smufl-name="noteheadClusterDoubleWhole3rd"/>-->
            <!--<glyph glyph-code="E129" smufl-name="noteheadClusterWhole3rd"/>-->
            <!--<glyph glyph-code="E12A" smufl-name="noteheadClusterHalf3rd"/>-->
            <!--<glyph glyph-code="E12B" smufl-name="noteheadClusterQuarter3rd"/>-->
            <!--<glyph glyph-code="E12C" smufl-name="noteheadClusterDoubleWholeTop"/>-->
            <!--<glyph glyph-code="E12D" smufl-name="noteheadClusterDoubleWholeMiddle"/>-->
            <!--<glyph glyph-code="E12E" smufl-name="noteheadClusterDoubleWholeBottom"/>-->
            <!--<glyph glyph-code="E12F" smufl-name="noteheadClusterWholeTop"/>-->
            <!--<glyph glyph-code="E130" smufl-name="noteheadClusterWholeMiddle"/>-->
            <!--<glyph glyph-code="E131" smufl-name="noteheadClusterWholeBottom"/>-->
            <!--<glyph glyph-code="E132" smufl-name="noteheadClusterHalfTop"/>-->
            <!--<glyph glyph-code="E133" smufl-name="noteheadClusterHalfMiddle"/>-->
            <!--<glyph glyph-code="E134" smufl-name="noteheadClusterHalfBottom"/>-->
            <!--<glyph glyph-code="E135" smufl-name="noteheadClusterQuarterTop"/>-->
            <!--<glyph glyph-code="E136" smufl-name="noteheadClusterQuarterMiddle"/>-->
            <!--<glyph glyph-code="E137" smufl-name="noteheadClusterQuarterBottom"/>-->
            <!--<glyph glyph-code="E138" smufl-name="noteheadDiamondClusterWhite2nd"/>-->
            <!--<glyph glyph-code="E139" smufl-name="noteheadDiamondClusterBlack2nd"/>-->
            <!--<glyph glyph-code="E13A" smufl-name="noteheadDiamondClusterWhite3rd"/>-->
            <!--<glyph glyph-code="E13B" smufl-name="noteheadDiamondClusterBlack3rd"/>-->
            <!--<glyph glyph-code="E13C" smufl-name="noteheadDiamondClusterWhiteTop"/>-->
            <!--<glyph glyph-code="E13D" smufl-name="noteheadDiamondClusterWhiteMiddle"/>-->
            <!--<glyph glyph-code="E13E" smufl-name="noteheadDiamondClusterWhiteBottom"/>-->
            <!--<glyph glyph-code="E13F" smufl-name="noteheadDiamondClusterBlackTop"/>-->
            <!--<glyph glyph-code="E140" smufl-name="noteheadDiamondClusterBlackMiddle"/>-->
            <!--<glyph glyph-code="E141" smufl-name="noteheadDiamondClusterBlackBottom"/>-->
            <!--<glyph glyph-code="E142" smufl-name="noteheadRectangularClusterBlackTop"/>-->
            <!--<glyph glyph-code="E143" smufl-name="noteheadRectangularClusterBlackMiddle"/>-->
            <!--<glyph glyph-code="E144" smufl-name="noteheadRectangularClusterBlackBottom"/>-->
            <!--<glyph glyph-code="E145" smufl-name="noteheadRectangularClusterWhiteTop"/>-->
            <!--<glyph glyph-code="E146" smufl-name="noteheadRectangularClusterWhiteMiddle"/>-->
            <!--<glyph glyph-code="E147" smufl-name="noteheadRectangularClusterWhiteBottom"/>-->
            <range_end>U+E14F</range_end>
            <range_start>U+E120</range_start>
        </noteClusters>
        <noteNameNoteheads>
            <description>Note name noteheads</description>
            <!--<glyph glyph-code="E150" smufl-name="noteDoWhole"/>-->
            <!--<glyph glyph-code="E151" smufl-name="noteReWhole"/>-->
            <!--<glyph glyph-code="E152" smufl-name="noteMiWhole"/>-->
            <!--<glyph glyph-code="E153" smufl-name="noteFaWhole"/>-->
            <!--<glyph glyph-code="E154" smufl-name="noteSoWhole"/>-->
            <!--<glyph glyph-code="E155" smufl-name="noteLaWhole"/>-->
            <!--<glyph glyph-code="E156" smufl-name="noteTiWhole"/>-->
            <!--<glyph glyph-code="E157" smufl-name="noteSiWhole"/>-->
            <!--<glyph glyph-code="E158" smufl-name="noteDoHalf"/>-->
            <!--<glyph glyph-code="E159" smufl-name="noteReHalf"/>-->
            <!--<glyph glyph-code="E15A" smufl-name="noteMiHalf"/>-->
            <!--<glyph glyph-code="E15B" smufl-name="noteFaHalf"/>-->
            <!--<glyph glyph-code="E15C" smufl-name="noteSoHalf"/>-->
            <!--<glyph glyph-code="E15D" smufl-name="noteLaHalf"/>-->
            <!--<glyph glyph-code="E15E" smufl-name="noteTiHalf"/>-->
            <!--<glyph glyph-code="E15F" smufl-name="noteSiHalf"/>-->
            <!--<glyph glyph-code="E160" smufl-name="noteDoBlack"/>-->
            <!--<glyph glyph-code="E161" smufl-name="noteReBlack"/>-->
            <!--<glyph glyph-code="E162" smufl-name="noteMiBlack"/>-->
            <!--<glyph glyph-code="E163" smufl-name="noteFaBlack"/>-->
            <!--<glyph glyph-code="E164" smufl-name="noteSoBlack"/>-->
            <!--<glyph glyph-code="E165" smufl-name="noteLaBlack"/>-->
            <!--<glyph glyph-code="E166" smufl-name="noteTiBlack"/>-->
            <!--<glyph glyph-code="E167" smufl-name="noteSiBlack"/>-->
            <!--<glyph glyph-code="E168" smufl-name="noteAFlatWhole"/>-->
            <!--<glyph glyph-code="E169" smufl-name="noteAWhole"/>-->
            <!--<glyph glyph-code="E16A" smufl-name="noteASharpWhole"/>-->
            <!--<glyph glyph-code="E16B" smufl-name="noteBFlatWhole"/>-->
            <!--<glyph glyph-code="E16C" smufl-name="noteBWhole"/>-->
            <!--<glyph glyph-code="E16D" smufl-name="noteBSharpWhole"/>-->
            <!--<glyph glyph-code="E16E" smufl-name="noteCFlatWhole"/>-->
            <!--<glyph glyph-code="E16F" smufl-name="noteCWhole"/>-->
            <!--<glyph glyph-code="E170" smufl-name="noteCSharpWhole"/>-->
            <!--<glyph glyph-code="E171" smufl-name="noteDFlatWhole"/>-->
            <!--<glyph glyph-code="E172" smufl-name="noteDWhole"/>-->
            <!--<glyph glyph-code="E173" smufl-name="noteDSharpWhole"/>-->
            <!--<glyph glyph-code="E174" smufl-name="noteEFlatWhole"/>-->
            <!--<glyph glyph-code="E175" smufl-name="noteEWhole"/>-->
            <!--<glyph glyph-code="E176" smufl-name="noteESharpWhole"/>-->
            <!--<glyph glyph-code="E177" smufl-name="noteFFlatWhole"/>-->
            <!--<glyph glyph-code="E178" smufl-name="noteFWhole"/>-->
            <!--<glyph glyph-code="E179" smufl-name="noteFSharpWhole"/>-->
            <!--<glyph glyph-code="E17A" smufl-name="noteGFlatWhole"/>-->
            <!--<glyph glyph-code="E17B" smufl-name="noteGWhole"/>-->
            <!--<glyph glyph-code="E17C" smufl-name="noteGSharpWhole"/>-->
            <!--<glyph glyph-code="E17D" smufl-name="noteHWhole"/>-->
            <!--<glyph glyph-code="E17E" smufl-name="noteHSharpWhole"/>-->
            <!--<glyph glyph-code="E17F" smufl-name="noteAFlatHalf"/>-->
            <!--<glyph glyph-code="E180" smufl-name="noteAHalf"/>-->
            <!--<glyph glyph-code="E181" smufl-name="noteASharpHalf"/>-->
            <!--<glyph glyph-code="E182" smufl-name="noteBFlatHalf"/>-->
            <!--<glyph glyph-code="E183" smufl-name="noteBHalf"/>-->
            <!--<glyph glyph-code="E184" smufl-name="noteBSharpHalf"/>-->
            <!--<glyph glyph-code="E185" smufl-name="noteCFlatHalf"/>-->
            <!--<glyph glyph-code="E186" smufl-name="noteCHalf"/>-->
            <!--<glyph glyph-code="E187" smufl-name="noteCSharpHalf"/>-->
            <!--<glyph glyph-code="E188" smufl-name="noteDFlatHalf"/>-->
            <!--<glyph glyph-code="E189" smufl-name="noteDHalf"/>-->
            <!--<glyph glyph-code="E18A" smufl-name="noteDSharpHalf"/>-->
            <!--<glyph glyph-code="E18B" smufl-name="noteEFlatHalf"/>-->
            <!--<glyph glyph-code="E18C" smufl-name="noteEHalf"/>-->
            <!--<glyph glyph-code="E18D" smufl-name="noteESharpHalf"/>-->
            <!--<glyph glyph-code="E18E" smufl-name="noteFFlatHalf"/>-->
            <!--<glyph glyph-code="E18F" smufl-name="noteFHalf"/>-->
            <!--<glyph glyph-code="E190" smufl-name="noteFSharpHalf"/>-->
            <!--<glyph glyph-code="E191" smufl-name="noteGFlatHalf"/>-->
            <!--<glyph glyph-code="E192" smufl-name="noteGHalf"/>-->
            <!--<glyph glyph-code="E193" smufl-name="noteGSharpHalf"/>-->
            <!--<glyph glyph-code="E194" smufl-name="noteHHalf"/>-->
            <!--<glyph glyph-code="E195" smufl-name="noteHSharpHalf"/>-->
            <!--<glyph glyph-code="E196" smufl-name="noteAFlatBlack"/>-->
            <!--<glyph glyph-code="E197" smufl-name="noteABlack"/>-->
            <!--<glyph glyph-code="E198" smufl-name="noteASharpBlack"/>-->
            <!--<glyph glyph-code="E199" smufl-name="noteBFlatBlack"/>-->
            <!--<glyph glyph-code="E19A" smufl-name="noteBBlack"/>-->
            <!--<glyph glyph-code="E19B" smufl-name="noteBSharpBlack"/>-->
            <!--<glyph glyph-code="E19C" smufl-name="noteCFlatBlack"/>-->
            <!--<glyph glyph-code="E19D" smufl-name="noteCBlack"/>-->
            <!--<glyph glyph-code="E19E" smufl-name="noteCSharpBlack"/>-->
            <!--<glyph glyph-code="E19F" smufl-name="noteDFlatBlack"/>-->
            <!--<glyph glyph-code="E1A0" smufl-name="noteDBlack"/>-->
            <!--<glyph glyph-code="E1A1" smufl-name="noteDSharpBlack"/>-->
            <!--<glyph glyph-code="E1A2" smufl-name="noteEFlatBlack"/>-->
            <!--<glyph glyph-code="E1A3" smufl-name="noteEBlack"/>-->
            <!--<glyph glyph-code="E1A4" smufl-name="noteESharpBlack"/>-->
            <!--<glyph glyph-code="E1A5" smufl-name="noteFFlatBlack"/>-->
            <!--<glyph glyph-code="E1A6" smufl-name="noteFBlack"/>-->
            <!--<glyph glyph-code="E1A7" smufl-name="noteFSharpBlack"/>-->
            <!--<glyph glyph-code="E1A8" smufl-name="noteGFlatBlack"/>-->
            <!--<glyph glyph-code="E1A9" smufl-name="noteGBlack"/>-->
            <!--<glyph glyph-code="E1AA" smufl-name="noteGSharpBlack"/>-->
            <!--<glyph glyph-code="E1AB" smufl-name="noteHBlack"/>-->
            <!--<glyph glyph-code="E1AC" smufl-name="noteHSharpBlack"/>-->
            <!--<glyph glyph-code="E1AD" smufl-name="noteEmptyWhole"/>-->
            <!--<glyph glyph-code="E1AE" smufl-name="noteEmptyHalf"/>-->
            <!--<glyph glyph-code="E1AF" smufl-name="noteEmptyBlack"/>-->
            <range_end>U+E1AF</range_end>
            <range_start>U+E150</range_start>
        </noteNameNoteheads>
        <shapeNoteNoteheads>
            <description>Shape note noteheads</description>
            <!--<glyph glyph-code="E1B0" smufl-name="noteShapeRoundWhite"/>-->
            <!--<glyph glyph-code="E1B1" smufl-name="noteShapeRoundBlack"/>-->
            <!--<glyph glyph-code="E1B2" smufl-name="noteShapeSquareWhite"/>-->
            <!--<glyph glyph-code="E1B3" smufl-name="noteShapeSquareBlack"/>-->
            <!--<glyph glyph-code="E1B4" smufl-name="noteShapeTriangleRightWhite"/>-->
            <!--<glyph glyph-code="E1B5" smufl-name="noteShapeTriangleRightBlack"/>-->
            <!--<glyph glyph-code="E1B6" smufl-name="noteShapeTriangleLeftWhite"/>-->
            <!--<glyph glyph-code="E1B7" smufl-name="noteShapeTriangleLeftBlack"/>-->
            <!--<glyph glyph-code="E1B8" smufl-name="noteShapeDiamondWhite"/>-->
            <!--<glyph glyph-code="E1B9" smufl-name="noteShapeDiamondBlack"/>-->
            <!--<glyph glyph-code="E1BA" smufl-name="noteShapeTriangleUpWhite"/>-->
            <!--<glyph glyph-code="E1BB" smufl-name="noteShapeTriangleUpBlack"/>-->
            <!--<glyph glyph-code="E1BC" smufl-name="noteShapeMoonWhite"/>-->
            <!--<glyph glyph-code="E1BD" smufl-name="noteShapeMoonBlack"/>-->
            <!--<glyph glyph-code="E1BE" smufl-name="noteShapeTriangleRoundWhite"/>-->
            <!--<glyph glyph-code="E1BF" smufl-name="noteShapeTriangleRoundBlack"/>-->
            <!--<glyph glyph-code="E1C0" smufl-name="noteShapeKeystoneWhite"/>-->
            <!--<glyph glyph-code="E1C1" smufl-name="noteShapeKeystoneBlack"/>-->
            <!--<glyph glyph-code="E1C2" smufl-name="noteShapeQuarterMoonWhite"/>-->
            <!--<glyph glyph-code="E1C3" smufl-name="noteShapeQuarterMoonBlack"/>-->
            <!--<glyph glyph-code="E1C4" smufl-name="noteShapeIsoscelesTriangleWhite"/>-->
            <!--<glyph glyph-code="E1C5" smufl-name="noteShapeIsoscelesTriangleBlack"/>-->
            <!--<glyph glyph-code="E1C6" smufl-name="noteShapeMoonLeftWhite"/>-->
            <!--<glyph glyph-code="E1C7" smufl-name="noteShapeMoonLeftBlack"/>-->
            <!--<glyph glyph-code="E1C8" smufl-name="noteShapeArrowheadLeftWhite"/>-->
            <!--<glyph glyph-code="E1C9" smufl-name="noteShapeArrowheadLeftBlack"/>-->
            <!--<glyph glyph-code="E1CA" smufl-name="noteShapeTriangleRoundLeftWhite"/>-->
            <!--<glyph glyph-code="E1CB" smufl-name="noteShapeTriangleRoundLeftBlack"/>-->
            <range_end>U+E1CF</range_end>
            <range_start>U+E1B0</range_start>
        </shapeNoteNoteheads>
        <individualNotes>
            <description>Individual notes</description>
            <!--<glyph glyph-code="E1D0" smufl-name="noteDoubleWhole"/>-->
            <!--<glyph glyph-code="E1D1" smufl-name="noteDoubleWholeSquare"/>-->
            <!--<glyph glyph-code="E1D2" smufl-name="noteWhole"/>-->
            <!--<glyph glyph-code="E1D3" smufl-name="noteHalfUp"/>-->
            <!--<glyph glyph-code="E1D4" smufl-name="noteHalfDown"/>-->
            <!--<glyph glyph-code="E1D5" smufl-name="noteQuarterUp"/>-->
            <!--<glyph glyph-code="E1D6" smufl-name="noteQuarterDown"/>-->
            <!--<glyph glyph-code="E1D7" smufl-name="note8thUp"/>-->
            <!--<glyph glyph-code="E1D8" smufl-name="note8thDown"/>-->
            <!--<glyph glyph-code="E1D9" smufl-name="note16thUp"/>-->
            <!--<glyph glyph-code="E1DA" smufl-name="note16thDown"/>-->
            <!--<glyph glyph-code="E1DB" smufl-name="note32ndUp"/>-->
            <!--<glyph glyph-code="E1DC" smufl-name="note32ndDown"/>-->
            <!--<glyph glyph-code="E1DD" smufl-name="note64thUp"/>-->
            <!--<glyph glyph-code="E1DE" smufl-name="note64thDown"/>-->
            <!--<glyph glyph-code="E1DF" smufl-name="note128thUp"/>-->
            <!--<glyph glyph-code="E1E0" smufl-name="note128thDown"/>-->
            <!--<glyph glyph-code="E1E1" smufl-name="note256thUp"/>-->
            <!--<glyph glyph-code="E1E2" smufl-name="note256thDown"/>-->
            <!--<glyph glyph-code="E1E3" smufl-name="note512thUp"/>-->
            <!--<glyph glyph-code="E1E4" smufl-name="note512thDown"/>-->
            <!--<glyph glyph-code="E1E5" smufl-name="note1024thUp"/>-->
            <!--<glyph glyph-code="E1E6" smufl-name="note1024thDown"/>-->
            <!--<glyph glyph-code="E1E7" smufl-name="augmentationDot"/>-->
            <range_end>U+E1EF</range_end>
            <range_start>U+E1D0</range_start>
        </individualNotes>
        <beamedGroupsOfNotes>
            <description>Beamed groups of notes</description>
            <!--<glyph glyph-code="E1F0" smufl-name="textBlackNoteShortStem"/>-->
            <!--<glyph glyph-code="E1F1" smufl-name="textBlackNoteLongStem"/>-->
            <!--<glyph glyph-code="E1F2" smufl-name="textBlackNoteFrac8thShortStem"/>-->
            <!--<glyph glyph-code="E1F3" smufl-name="textBlackNoteFrac8thLongStem"/>-->
            <!--<glyph glyph-code="E1F4" smufl-name="textBlackNoteFrac16thShortStem"/>-->
            <!--<glyph glyph-code="E1F5" smufl-name="textBlackNoteFrac16thLongStem"/>-->
            <!--<glyph glyph-code="E1F6" smufl-name="textBlackNoteFrac32ndLongStem"/>-->
            <!--<glyph glyph-code="E1F7" smufl-name="textCont8thBeamShortStem"/>-->
            <!--<glyph glyph-code="E1F8" smufl-name="textCont8thBeamLongStem"/>-->
            <!--<glyph glyph-code="E1F9" smufl-name="textCont16thBeamShortStem"/>-->
            <!--<glyph glyph-code="E1FA" smufl-name="textCont16thBeamLongStem"/>-->
            <!--<glyph glyph-code="E1FB" smufl-name="textCont32ndBeamLongStem"/>-->
            <!--<glyph glyph-code="E1FC" smufl-name="textAugmentationDot"/>-->
            <!--<glyph glyph-code="E1FD" smufl-name="textTie"/>-->
            <!--<glyph glyph-code="E1FE" smufl-name="textTupletBracketStartShortStem"/>-->
            <!--<glyph glyph-code="E1FF" smufl-name="textTuplet3ShortStem"/>-->
            <!--<glyph glyph-code="E200" smufl-name="textTupletBracketEndShortStem"/>-->
            <!--<glyph glyph-code="E201" smufl-name="textTupletBracketStartLongStem"/>-->
            <!--<glyph glyph-code="E202" smufl-name="textTuplet3LongStem"/>-->
            <!--<glyph glyph-code="E203" smufl-name="textTupletBracketEndLongStem"/>-->
            <range_end>U+E20F</range_end>
            <range_start>U+E1F0</range_start>
        </beamedGroupsOfNotes>
        <stems>
            <description>Stems</description>
            <!--<glyph glyph-code="E210" smufl-name="stem"/>-->
            <!--<glyph glyph-code="E211" smufl-name="stemSprechgesang"/>-->
            <!--<glyph glyph-code="E212" smufl-name="stemSwished"/>-->
            <!--<glyph glyph-code="E213" smufl-name="stemPendereckiTremolo"/>-->
            <!--<glyph glyph-code="E214" smufl-name="stemSulPonticello"/>-->
            <!--<glyph glyph-code="E215" smufl-name="stemBowOnBridge"/>-->
            <!--<glyph glyph-code="E216" smufl-name="stemBowOnTailpiece"/>-->
            <!--<glyph glyph-code="E217" smufl-name="stemBuzzRoll"/>-->
            <!--<glyph glyph-code="E218" smufl-name="stemDamp"/>-->
            <!--<glyph glyph-code="E219" smufl-name="stemVibratoPulse"/>-->
            <!--<glyph glyph-code="E21A" smufl-name="stemMultiphonicsBlack"/>-->
            <!--<glyph glyph-code="E21B" smufl-name="stemMultiphonicsWhite"/>-->
            <!--<glyph glyph-code="E21C" smufl-name="stemMultiphonicsBlackWhite"/>-->
            <!--<glyph glyph-code="E21D" smufl-name="stemSussurando"/>-->
            <!--<glyph glyph-code="E21E" smufl-name="stemRimShot"/>-->
            <!--<glyph glyph-code="E21F" smufl-name="stemHarpStringNoise"/>-->
            <range_end>U+E21F</range_end>
            <range_start>U+E210</range_start>
        </stems>
        <tremolos>
            <description>Tremolos</description>
            <glyph glyph-code="E220" smufl-name="tremolo1"/>
            <glyph glyph-code="E221" smufl-name="tremolo2"/>
            <glyph glyph-code="E222" smufl-name="tremolo3"/>
            <glyph glyph-code="E223" smufl-name="tremolo4"/>
            <glyph glyph-code="E224" smufl-name="tremolo5"/>
            <!--<glyph glyph-code="E225" smufl-name="tremoloFingered1"/>-->
            <!--<glyph glyph-code="E226" smufl-name="tremoloFingered2"/>-->
            <!--<glyph glyph-code="E227" smufl-name="tremoloFingered3"/>-->
            <!--<glyph glyph-code="E228" smufl-name="tremoloFingered4"/>-->
            <!--<glyph glyph-code="E229" smufl-name="tremoloFingered5"/>-->
            <!--<glyph glyph-code="E22A" smufl-name="buzzRoll"/>-->
            <!--<glyph glyph-code="E22B" smufl-name="pendereckiTremolo"/>-->
            <!--<glyph glyph-code="E22C" smufl-name="unmeasuredTremolo"/>-->
            <!--<glyph glyph-code="E22D" smufl-name="unmeasuredTremoloSimple"/>-->
            <!--<glyph glyph-code="E22E" smufl-name="tremoloDivisiDots2"/>-->
            <!--<glyph glyph-code="E22F" smufl-name="tremoloDivisiDots3"/>-->
            <!--<glyph glyph-code="E230" smufl-name="tremoloDivisiDots4"/>-->
            <!--<glyph glyph-code="E231" smufl-name="tremoloDivisiDots6"/>-->
            <range_end>U+E23F</range_end>
            <range_start>U+E220</range_start>
        </tremolos>
        <flags>
            <description>Flags</description>
            <glyph glyph-code="E240" smufl-name="flag8thUp"/>
            <glyph glyph-code="E241" smufl-name="flag8thDown"/>
            <glyph glyph-code="E242" smufl-name="flag16thUp"/>
            <glyph glyph-code="E243" smufl-name="flag16thDown"/>
            <glyph glyph-code="E244" smufl-name="flag32ndUp"/>
            <glyph glyph-code="E245" smufl-name="flag32ndDown"/>
            <glyph glyph-code="E246" smufl-name="flag64thUp"/>
            <glyph glyph-code="E247" smufl-name="flag64thDown"/>
            <glyph glyph-code="E248" smufl-name="flag128thUp"/>
            <glyph glyph-code="E249" smufl-name="flag128thDown"/>
            <glyph glyph-code="E24A" smufl-name="flag256thUp"/>
            <glyph glyph-code="E24B" smufl-name="flag256thDown"/>
            <glyph glyph-code="E24C" smufl-name="flag512thUp"/>
            <glyph glyph-code="E24D" smufl-name="flag512thDown"/>
            <glyph glyph-code="E24E" smufl-name="flag1024thUp"/>
            <glyph glyph-code="E24F" smufl-name="flag1024thDown"/>
            <!--<glyph glyph-code="E250" smufl-name="flagInternalUp"/>-->
            <!--<glyph glyph-code="E251" smufl-name="flagInternalDown"/>-->
            <range_end>U+E25F</range_end>
            <range_start>U+E240</range_start>
        </flags>
        <standardAccidentals12Edo>
            <description>Standard accidentals (12-EDO)</description>
            <glyph glyph-code="E260" smufl-name="accidentalFlat"/>
            <glyph glyph-code="E261" smufl-name="accidentalNatural"/>
            <glyph glyph-code="E262" smufl-name="accidentalSharp"/>
            <glyph glyph-code="E263" smufl-name="accidentalDoubleSharp"/>
            <glyph glyph-code="E264" smufl-name="accidentalDoubleFlat"/>
            <glyph glyph-code="E265" smufl-name="accidentalTripleSharp"/>
            <glyph glyph-code="E266" smufl-name="accidentalTripleFlat"/>
            <glyph glyph-code="E267" smufl-name="accidentalNaturalFlat"/>
            <glyph glyph-code="E268" smufl-name="accidentalNaturalSharp"/>
            <glyph glyph-code="E269" smufl-name="accidentalSharpSharp"/>
            <glyph glyph-code="E26A" smufl-name="accidentalParensLeft"/>
            <glyph glyph-code="E26B" smufl-name="accidentalParensRight"/>
            <glyph glyph-code="E26C" smufl-name="accidentalBracketLeft"/>
            <glyph glyph-code="E26D" smufl-name="accidentalBracketRight"/>
            <range_end>U+E26F</range_end>
            <range_start>U+E260</range_start>
        </standardAccidentals12Edo>
        <gouldArrowQuartertoneAccidentals24Edo>
            <description>Gould arrow quartertone accidentals (24-EDO)</description>
            <glyph glyph-code="E270" smufl-name="accidentalQuarterToneFlatArrowUp"/>
            <glyph glyph-code="E271" smufl-name="accidentalThreeQuarterTonesFlatArrowDown"/>
            <glyph glyph-code="E272" smufl-name="accidentalQuarterToneSharpNaturalArrowUp"/>
            <glyph glyph-code="E273"
                smufl-name="accidentalQuarterToneFlatNaturalArrowDown"/>
            <glyph glyph-code="E274" smufl-name="accidentalThreeQuarterTonesSharpArrowUp"/>
            <glyph glyph-code="E275" smufl-name="accidentalQuarterToneSharpArrowDown"/>
            <!--<glyph glyph-code="E276" smufl-name="accidentalFiveQuarterTonesSharpArrowUp"/>-->
            <!--<glyph glyph-code="E277"
                smufl-name="accidentalThreeQuarterTonesSharpArrowDown"/>-->
            <!--<glyph glyph-code="E278" smufl-name="accidentalThreeQuarterTonesFlatArrowUp"/>-->
            <!--<glyph glyph-code="E279" smufl-name="accidentalFiveQuarterTonesFlatArrowDown"/>-->
            <!--<glyph glyph-code="E27A" smufl-name="accidentalArrowUp"/>-->
            <!--<glyph glyph-code="E27B" smufl-name="accidentalArrowDown"/>-->
            <range_end>U+E27F</range_end>
            <range_start>U+E270</range_start>
        </gouldArrowQuartertoneAccidentals24Edo>
        <steinZimmermannAccidentals24Edo>
            <description>Stein-Zimmermann accidentals (24-EDO)</description>
            <glyph glyph-code="E280" smufl-name="accidentalQuarterToneFlatStein"/>
            <glyph glyph-code="E281" smufl-name="accidentalThreeQuarterTonesFlatZimmermann"/>
            <glyph glyph-code="E282" smufl-name="accidentalQuarterToneSharpStein"/>
            <glyph glyph-code="E283" smufl-name="accidentalThreeQuarterTonesSharpStein"/>
            <!--<glyph glyph-code="E284" smufl-name="accidentalNarrowReversedFlat"/>-->
            <!--<glyph glyph-code="E285" smufl-name="accidentalNarrowReversedFlatAndFlat"/>-->
            <range_end>U+E28F</range_end>
            <range_start>U+E280</range_start>
        </steinZimmermannAccidentals24Edo>
        <extendedSteinZimmermannAccidentals>
            <description>Extended Stein-Zimmermann accidentals</description>
            <!--<glyph glyph-code="E290" smufl-name="accidentalReversedFlatArrowUp"/>-->
            <!--<glyph glyph-code="E291" smufl-name="accidentalReversedFlatArrowDown"/>-->
            <!--<glyph glyph-code="E292" smufl-name="accidentalFilledReversedFlatArrowUp"/>-->
            <!--<glyph glyph-code="E293" smufl-name="accidentalFilledReversedFlatArrowDown"/>-->
            <!--<glyph glyph-code="E294" smufl-name="accidentalReversedFlatAndFlatArrowUp"/>-->
            <!--<glyph glyph-code="E295" smufl-name="accidentalReversedFlatAndFlatArrowDown"/>-->
            <!--<glyph glyph-code="E296" smufl-name="accidentalFilledReversedFlatAndFlat"/>-->
            <!--<glyph glyph-code="E297"
                smufl-name="accidentalFilledReversedFlatAndFlatArrowUp"/>-->
            <!--<glyph glyph-code="E298"
                smufl-name="accidentalFilledReversedFlatAndFlatArrowDown"/>-->
            <!--<glyph glyph-code="E299" smufl-name="accidentalHalfSharpArrowUp"/>-->
            <!--<glyph glyph-code="E29A" smufl-name="accidentalHalfSharpArrowDown"/>-->
            <!--<glyph glyph-code="E29B" smufl-name="accidentalOneAndAHalfSharpsArrowUp"/>-->
            <!--<glyph glyph-code="E29C" smufl-name="accidentalOneAndAHalfSharpsArrowDown"/>-->
            <range_end>U+E29F</range_end>
            <range_start>U+E290</range_start>
        </extendedSteinZimmermannAccidentals>
        <simsAccidentals72Edo>
            <description>Sims accidentals (72-EDO)</description>
            <!--<glyph glyph-code="E2A0" smufl-name="accidentalSims12Down"/>-->
            <!--<glyph glyph-code="E2A1" smufl-name="accidentalSims6Down"/>-->
            <!--<glyph glyph-code="E2A2" smufl-name="accidentalSims4Down"/>-->
            <!--<glyph glyph-code="E2A3" smufl-name="accidentalSims12Up"/>-->
            <!--<glyph glyph-code="E2A4" smufl-name="accidentalSims6Up"/>-->
            <!--<glyph glyph-code="E2A5" smufl-name="accidentalSims4Up"/>-->
            <range_end>U+E2AF</range_end>
            <range_start>U+E2A0</range_start>
        </simsAccidentals72Edo>
        <johnstonAccidentalsJustIntonation>
            <description>Johnston accidentals (just intonation)</description>
            <!--<glyph glyph-code="E2B0" smufl-name="accidentalJohnstonPlus"/>-->
            <!--<glyph glyph-code="E2B1" smufl-name="accidentalJohnstonMinus"/>-->
            <!--<glyph glyph-code="E2B2" smufl-name="accidentalJohnstonEl"/>-->
            <!--<glyph glyph-code="E2B3" smufl-name="accidentalJohnstonSeven"/>-->
            <!--<glyph glyph-code="E2B4" smufl-name="accidentalJohnstonUp"/>-->
            <!--<glyph glyph-code="E2B5" smufl-name="accidentalJohnstonDown"/>-->
            <!--<glyph glyph-code="E2B6" smufl-name="accidentalJohnston13"/>-->
            <!--<glyph glyph-code="E2B7" smufl-name="accidentalJohnston31"/>-->
            <range_end>U+E2BF</range_end>
            <range_start>U+E2B0</range_start>
        </johnstonAccidentalsJustIntonation>
        <extendedHelmholtzEllisAccidentalsJustIntonation>
            <description>Extended Helmholtz-Ellis accidentals (just intonation)</description>
            <!--<glyph glyph-code="E2C0" smufl-name="accidentalDoubleFlatOneArrowDown"/>-->
            <!--<glyph glyph-code="E2C1" smufl-name="accidentalFlatOneArrowDown"/>-->
            <!--<glyph glyph-code="E2C2" smufl-name="accidentalNaturalOneArrowDown"/>-->
            <!--<glyph glyph-code="E2C3" smufl-name="accidentalSharpOneArrowDown"/>-->
            <!--<glyph glyph-code="E2C4" smufl-name="accidentalDoubleSharpOneArrowDown"/>-->
            <!--<glyph glyph-code="E2C5" smufl-name="accidentalDoubleFlatOneArrowUp"/>-->
            <!--<glyph glyph-code="E2C6" smufl-name="accidentalFlatOneArrowUp"/>-->
            <!--<glyph glyph-code="E2C7" smufl-name="accidentalNaturalOneArrowUp"/>-->
            <!--<glyph glyph-code="E2C8" smufl-name="accidentalSharpOneArrowUp"/>-->
            <!--<glyph glyph-code="E2C9" smufl-name="accidentalDoubleSharpOneArrowUp"/>-->
            <!--<glyph glyph-code="E2CA" smufl-name="accidentalDoubleFlatTwoArrowsDown"/>-->
            <!--<glyph glyph-code="E2CB" smufl-name="accidentalFlatTwoArrowsDown"/>-->
            <!--<glyph glyph-code="E2CC" smufl-name="accidentalNaturalTwoArrowsDown"/>-->
            <!--<glyph glyph-code="E2CD" smufl-name="accidentalSharpTwoArrowsDown"/>-->
            <!--<glyph glyph-code="E2CE" smufl-name="accidentalDoubleSharpTwoArrowsDown"/>-->
            <!--<glyph glyph-code="E2CF" smufl-name="accidentalDoubleFlatTwoArrowsUp"/>-->
            <!--<glyph glyph-code="E2D0" smufl-name="accidentalFlatTwoArrowsUp"/>-->
            <!--<glyph glyph-code="E2D1" smufl-name="accidentalNaturalTwoArrowsUp"/>-->
            <!--<glyph glyph-code="E2D2" smufl-name="accidentalSharpTwoArrowsUp"/>-->
            <!--<glyph glyph-code="E2D3" smufl-name="accidentalDoubleSharpTwoArrowsUp"/>-->
            <!--<glyph glyph-code="E2D4" smufl-name="accidentalDoubleFlatThreeArrowsDown"/>-->
            <!--<glyph glyph-code="E2D5" smufl-name="accidentalFlatThreeArrowsDown"/>-->
            <!--<glyph glyph-code="E2D6" smufl-name="accidentalNaturalThreeArrowsDown"/>-->
            <!--<glyph glyph-code="E2D7" smufl-name="accidentalSharpThreeArrowsDown"/>-->
            <!--<glyph glyph-code="E2D8" smufl-name="accidentalDoubleSharpThreeArrowsDown"/>-->
            <!--<glyph glyph-code="E2D9" smufl-name="accidentalDoubleFlatThreeArrowsUp"/>-->
            <!--<glyph glyph-code="E2DA" smufl-name="accidentalFlatThreeArrowsUp"/>-->
            <!--<glyph glyph-code="E2DB" smufl-name="accidentalNaturalThreeArrowsUp"/>-->
            <!--<glyph glyph-code="E2DC" smufl-name="accidentalSharpThreeArrowsUp"/>-->
            <!--<glyph glyph-code="E2DD" smufl-name="accidentalDoubleSharpThreeArrowsUp"/>-->
            <!--<glyph glyph-code="E2DE" smufl-name="accidentalLowerOneSeptimalComma"/>-->
            <!--<glyph glyph-code="E2DF" smufl-name="accidentalRaiseOneSeptimalComma"/>-->
            <!--<glyph glyph-code="E2E0" smufl-name="accidentalLowerTwoSeptimalCommas"/>-->
            <!--<glyph glyph-code="E2E1" smufl-name="accidentalRaiseTwoSeptimalCommas"/>-->
            <!--<glyph glyph-code="E2E2" smufl-name="accidentalLowerOneUndecimalQuartertone"/>-->
            <!--<glyph glyph-code="E2E3" smufl-name="accidentalRaiseOneUndecimalQuartertone"/>-->
            <!--<glyph glyph-code="E2E4" smufl-name="accidentalLowerOneTridecimalQuartertone"/>-->
            <!--<glyph glyph-code="E2E5" smufl-name="accidentalRaiseOneTridecimalQuartertone"/>-->
            <!--<glyph glyph-code="E2E6" smufl-name="accidentalCombiningLower17Schisma"/>-->
            <!--<glyph glyph-code="E2E7" smufl-name="accidentalCombiningRaise17Schisma"/>-->
            <!--<glyph glyph-code="E2E8" smufl-name="accidentalCombiningLower19Schisma"/>-->
            <!--<glyph glyph-code="E2E9" smufl-name="accidentalCombiningRaise19Schisma"/>-->
            <!--<glyph glyph-code="E2EA"
                smufl-name="accidentalCombiningLower23Limit29LimitComma"/>-->
            <!--<glyph glyph-code="E2EB"
                smufl-name="accidentalCombiningRaise23Limit29LimitComma"/>-->
            <!--<glyph glyph-code="E2EC" smufl-name="accidentalCombiningLower31Schisma"/>-->
            <!--<glyph glyph-code="E2ED" smufl-name="accidentalCombiningRaise31Schisma"/>-->
            <!--<glyph glyph-code="E2EE" smufl-name="accidentalCombiningOpenCurlyBrace"/>-->
            <!--<glyph glyph-code="E2EF" smufl-name="accidentalCombiningCloseCurlyBrace"/>-->
            <!--<glyph glyph-code="E2F0" smufl-name="accidentalDoubleFlatEqualTempered"/>-->
            <!--<glyph glyph-code="E2F1" smufl-name="accidentalFlatEqualTempered"/>-->
            <!--<glyph glyph-code="E2F2" smufl-name="accidentalNaturalEqualTempered"/>-->
            <!--<glyph glyph-code="E2F3" smufl-name="accidentalSharpEqualTempered"/>-->
            <!--<glyph glyph-code="E2F4" smufl-name="accidentalDoubleSharpEqualTempered"/>-->
            <range_end>U+E2FF</range_end>
            <range_start>U+E2C0</range_start>
        </extendedHelmholtzEllisAccidentalsJustIntonation>
        <spartanSagittalSingleShaftAccidentals>
            <description>Spartan Sagittal single-shaft accidentals</description>
            <!--<glyph glyph-code="E300" smufl-name="accSagittal5v7KleismaUp"/>-->
            <!--<glyph glyph-code="E301" smufl-name="accSagittal5v7KleismaDown"/>-->
            <!--<glyph glyph-code="E302" smufl-name="accSagittal5CommaUp"/>-->
            <!--<glyph glyph-code="E303" smufl-name="accSagittal5CommaDown"/>-->
            <!--<glyph glyph-code="E304" smufl-name="accSagittal7CommaUp"/>-->
            <!--<glyph glyph-code="E305" smufl-name="accSagittal7CommaDown"/>-->
            <!--<glyph glyph-code="E306" smufl-name="accSagittal25SmallDiesisUp"/>-->
            <!--<glyph glyph-code="E307" smufl-name="accSagittal25SmallDiesisDown"/>-->
            <!--<glyph glyph-code="E308" smufl-name="accSagittal35MediumDiesisUp"/>-->
            <!--<glyph glyph-code="E309" smufl-name="accSagittal35MediumDiesisDown"/>-->
            <!--<glyph glyph-code="E30A" smufl-name="accSagittal11MediumDiesisUp"/>-->
            <!--<glyph glyph-code="E30B" smufl-name="accSagittal11MediumDiesisDown"/>-->
            <!--<glyph glyph-code="E30C" smufl-name="accSagittal11LargeDiesisUp"/>-->
            <!--<glyph glyph-code="E30D" smufl-name="accSagittal11LargeDiesisDown"/>-->
            <!--<glyph glyph-code="E30E" smufl-name="accSagittal35LargeDiesisUp"/>-->
            <!--<glyph glyph-code="E30F" smufl-name="accSagittal35LargeDiesisDown"/>-->
            <range_end>U+E30F</range_end>
            <range_start>U+E300</range_start>
        </spartanSagittalSingleShaftAccidentals>
        <spartanSagittalMultiShaftAccidentals>
            <description>Spartan Sagittal multi-shaft accidentals</description>
            <!--<glyph glyph-code="E310" smufl-name="accSagittalSharp25SDown"/>-->
            <!--<glyph glyph-code="E311" smufl-name="accSagittalFlat25SUp"/>-->
            <!--<glyph glyph-code="E312" smufl-name="accSagittalSharp7CDown"/>-->
            <!--<glyph glyph-code="E313" smufl-name="accSagittalFlat7CUp"/>-->
            <!--<glyph glyph-code="E314" smufl-name="accSagittalSharp5CDown"/>-->
            <!--<glyph glyph-code="E315" smufl-name="accSagittalFlat5CUp"/>-->
            <!--<glyph glyph-code="E316" smufl-name="accSagittalSharp5v7kDown"/>-->
            <!--<glyph glyph-code="E317" smufl-name="accSagittalFlat5v7kUp"/>-->
            <!--<glyph glyph-code="E318" smufl-name="accSagittalSharp"/>-->
            <!--<glyph glyph-code="E319" smufl-name="accSagittalFlat"/>-->
            <!--<glyph glyph-code="E31A" smufl-name="accSagittalUnused1"/>-->
            <!--<glyph glyph-code="E31B" smufl-name="accSagittalUnused2"/>-->
            <!--<glyph glyph-code="E31C" smufl-name="accSagittalSharp5v7kUp"/>-->
            <!--<glyph glyph-code="E31D" smufl-name="accSagittalFlat5v7kDown"/>-->
            <!--<glyph glyph-code="E31E" smufl-name="accSagittalSharp5CUp"/>-->
            <!--<glyph glyph-code="E31F" smufl-name="accSagittalFlat5CDown"/>-->
            <!--<glyph glyph-code="E320" smufl-name="accSagittalSharp7CUp"/>-->
            <!--<glyph glyph-code="E321" smufl-name="accSagittalFlat7CDown"/>-->
            <!--<glyph glyph-code="E322" smufl-name="accSagittalSharp25SUp"/>-->
            <!--<glyph glyph-code="E323" smufl-name="accSagittalFlat25SDown"/>-->
            <!--<glyph glyph-code="E324" smufl-name="accSagittalSharp35MUp"/>-->
            <!--<glyph glyph-code="E325" smufl-name="accSagittalFlat35MDown"/>-->
            <!--<glyph glyph-code="E326" smufl-name="accSagittalSharp11MUp"/>-->
            <!--<glyph glyph-code="E327" smufl-name="accSagittalFlat11MDown"/>-->
            <!--<glyph glyph-code="E328" smufl-name="accSagittalSharp11LUp"/>-->
            <!--<glyph glyph-code="E329" smufl-name="accSagittalFlat11LDown"/>-->
            <!--<glyph glyph-code="E32A" smufl-name="accSagittalSharp35LUp"/>-->
            <!--<glyph glyph-code="E32B" smufl-name="accSagittalFlat35LDown"/>-->
            <!--<glyph glyph-code="E32C" smufl-name="accSagittalDoubleSharp25SDown"/>-->
            <!--<glyph glyph-code="E32D" smufl-name="accSagittalDoubleFlat25SUp"/>-->
            <!--<glyph glyph-code="E32E" smufl-name="accSagittalDoubleSharp7CDown"/>-->
            <!--<glyph glyph-code="E32F" smufl-name="accSagittalDoubleFlat7CUp"/>-->
            <!--<glyph glyph-code="E330" smufl-name="accSagittalDoubleSharp5CDown"/>-->
            <!--<glyph glyph-code="E331" smufl-name="accSagittalDoubleFlat5CUp"/>-->
            <!--<glyph glyph-code="E332" smufl-name="accSagittalDoubleSharp5v7kDown"/>-->
            <!--<glyph glyph-code="E333" smufl-name="accSagittalDoubleFlat5v7kUp"/>-->
            <!--<glyph glyph-code="E334" smufl-name="accSagittalDoubleSharp"/>-->
            <!--<glyph glyph-code="E335" smufl-name="accSagittalDoubleFlat"/>-->
            <range_end>U+E33F</range_end>
            <range_start>U+E310</range_start>
        </spartanSagittalMultiShaftAccidentals>
        <athenianSagittalExtensionMediumPrecisionAccidentals>
            <description>Athenian Sagittal extension (medium precision) accidentals</description>
            <!--<glyph glyph-code="E340" smufl-name="accSagittal7v11KleismaUp"/>-->
            <!--<glyph glyph-code="E341" smufl-name="accSagittal7v11KleismaDown"/>-->
            <!--<glyph glyph-code="E342" smufl-name="accSagittal17CommaUp"/>-->
            <!--<glyph glyph-code="E343" smufl-name="accSagittal17CommaDown"/>-->
            <!--<glyph glyph-code="E344" smufl-name="accSagittal55CommaUp"/>-->
            <!--<glyph glyph-code="E345" smufl-name="accSagittal55CommaDown"/>-->
            <!--<glyph glyph-code="E346" smufl-name="accSagittal7v11CommaUp"/>-->
            <!--<glyph glyph-code="E347" smufl-name="accSagittal7v11CommaDown"/>-->
            <!--<glyph glyph-code="E348" smufl-name="accSagittal5v11SmallDiesisUp"/>-->
            <!--<glyph glyph-code="E349" smufl-name="accSagittal5v11SmallDiesisDown"/>-->
            <!--<glyph glyph-code="E34A" smufl-name="accSagittalSharp5v11SDown"/>-->
            <!--<glyph glyph-code="E34B" smufl-name="accSagittalFlat5v11SUp"/>-->
            <!--<glyph glyph-code="E34C" smufl-name="accSagittalSharp7v11CDown"/>-->
            <!--<glyph glyph-code="E34D" smufl-name="accSagittalFlat7v11CUp"/>-->
            <!--<glyph glyph-code="E34E" smufl-name="accSagittalSharp55CDown"/>-->
            <!--<glyph glyph-code="E34F" smufl-name="accSagittalFlat55CUp"/>-->
            <!--<glyph glyph-code="E350" smufl-name="accSagittalSharp17CDown"/>-->
            <!--<glyph glyph-code="E351" smufl-name="accSagittalFlat17CUp"/>-->
            <!--<glyph glyph-code="E352" smufl-name="accSagittalSharp7v11kDown"/>-->
            <!--<glyph glyph-code="E353" smufl-name="accSagittalFlat7v11kUp"/>-->
            <!--<glyph glyph-code="E354" smufl-name="accSagittalSharp7v11kUp"/>-->
            <!--<glyph glyph-code="E355" smufl-name="accSagittalFlat7v11kDown"/>-->
            <!--<glyph glyph-code="E356" smufl-name="accSagittalSharp17CUp"/>-->
            <!--<glyph glyph-code="E357" smufl-name="accSagittalFlat17CDown"/>-->
            <!--<glyph glyph-code="E358" smufl-name="accSagittalSharp55CUp"/>-->
            <!--<glyph glyph-code="E359" smufl-name="accSagittalFlat55CDown"/>-->
            <!--<glyph glyph-code="E35A" smufl-name="accSagittalSharp7v11CUp"/>-->
            <!--<glyph glyph-code="E35B" smufl-name="accSagittalFlat7v11CDown"/>-->
            <!--<glyph glyph-code="E35C" smufl-name="accSagittalSharp5v11SUp"/>-->
            <!--<glyph glyph-code="E35D" smufl-name="accSagittalFlat5v11SDown"/>-->
            <!--<glyph glyph-code="E35E" smufl-name="accSagittalDoubleSharp5v11SDown"/>-->
            <!--<glyph glyph-code="E35F" smufl-name="accSagittalDoubleFlat5v11SUp"/>-->
            <!--<glyph glyph-code="E360" smufl-name="accSagittalDoubleSharp7v11CDown"/>-->
            <!--<glyph glyph-code="E361" smufl-name="accSagittalDoubleFlat7v11CUp"/>-->
            <!--<glyph glyph-code="E362" smufl-name="accSagittalDoubleSharp55CDown"/>-->
            <!--<glyph glyph-code="E363" smufl-name="accSagittalDoubleFlat55CUp"/>-->
            <!--<glyph glyph-code="E364" smufl-name="accSagittalDoubleSharp17CDown"/>-->
            <!--<glyph glyph-code="E365" smufl-name="accSagittalDoubleFlat17CUp"/>-->
            <!--<glyph glyph-code="E366" smufl-name="accSagittalDoubleSharp7v11kDown"/>-->
            <!--<glyph glyph-code="E367" smufl-name="accSagittalDoubleFlat7v11kUp"/>-->
            <range_end>U+E36F</range_end>
            <range_start>U+E340</range_start>
        </athenianSagittalExtensionMediumPrecisionAccidentals>
        <trojanSagittalExtension12EdoRelativeAccidentals>
            <description>Trojan Sagittal extension (12-EDO relative) accidentals</description>
            <!--<glyph glyph-code="E370" smufl-name="accSagittal23CommaUp"/>-->
            <!--<glyph glyph-code="E371" smufl-name="accSagittal23CommaDown"/>-->
            <!--<glyph glyph-code="E372" smufl-name="accSagittal5v19CommaUp"/>-->
            <!--<glyph glyph-code="E373" smufl-name="accSagittal5v19CommaDown"/>-->
            <!--<glyph glyph-code="E374" smufl-name="accSagittal5v23SmallDiesisUp"/>-->
            <!--<glyph glyph-code="E375" smufl-name="accSagittal5v23SmallDiesisDown"/>-->
            <!--<glyph glyph-code="E376" smufl-name="accSagittalSharp5v23SDown"/>-->
            <!--<glyph glyph-code="E377" smufl-name="accSagittalFlat5v23SUp"/>-->
            <!--<glyph glyph-code="E378" smufl-name="accSagittalSharp5v19CDown"/>-->
            <!--<glyph glyph-code="E379" smufl-name="accSagittalFlat5v19CUp"/>-->
            <!--<glyph glyph-code="E37A" smufl-name="accSagittalSharp23CDown"/>-->
            <!--<glyph glyph-code="E37B" smufl-name="accSagittalFlat23CUp"/>-->
            <!--<glyph glyph-code="E37C" smufl-name="accSagittalSharp23CUp"/>-->
            <!--<glyph glyph-code="E37D" smufl-name="accSagittalFlat23CDown"/>-->
            <!--<glyph glyph-code="E37E" smufl-name="accSagittalSharp5v19CUp"/>-->
            <!--<glyph glyph-code="E37F" smufl-name="accSagittalFlat5v19CDown"/>-->
            <!--<glyph glyph-code="E380" smufl-name="accSagittalSharp5v23SUp"/>-->
            <!--<glyph glyph-code="E381" smufl-name="accSagittalFlat5v23SDown"/>-->
            <!--<glyph glyph-code="E382" smufl-name="accSagittalDoubleSharp5v23SDown"/>-->
            <!--<glyph glyph-code="E383" smufl-name="accSagittalDoubleFlat5v23SUp"/>-->
            <!--<glyph glyph-code="E384" smufl-name="accSagittalDoubleSharp5v19CDown"/>-->
            <!--<glyph glyph-code="E385" smufl-name="accSagittalDoubleFlat5v19CUp"/>-->
            <!--<glyph glyph-code="E386" smufl-name="accSagittalDoubleSharp23CDown"/>-->
            <!--<glyph glyph-code="E387" smufl-name="accSagittalDoubleFlat23CUp"/>-->
            <range_end>U+E38F</range_end>
            <range_start>U+E370</range_start>
        </trojanSagittalExtension12EdoRelativeAccidentals>
        <prometheanSagittalExtensionHighPrecisionSingleShaftAccidentals>
            <description>Promethean Sagittal extension (high precision) single-shaft accidentals</description>
            <!--<glyph glyph-code="E390" smufl-name="accSagittal19SchismaUp"/>-->
            <!--<glyph glyph-code="E391" smufl-name="accSagittal19SchismaDown"/>-->
            <!--<glyph glyph-code="E392" smufl-name="accSagittal17KleismaUp"/>-->
            <!--<glyph glyph-code="E393" smufl-name="accSagittal17KleismaDown"/>-->
            <!--<glyph glyph-code="E394" smufl-name="accSagittal143CommaUp"/>-->
            <!--<glyph glyph-code="E395" smufl-name="accSagittal143CommaDown"/>-->
            <!--<glyph glyph-code="E396" smufl-name="accSagittal11v49CommaUp"/>-->
            <!--<glyph glyph-code="E397" smufl-name="accSagittal11v49CommaDown"/>-->
            <!--<glyph glyph-code="E398" smufl-name="accSagittal19CommaUp"/>-->
            <!--<glyph glyph-code="E399" smufl-name="accSagittal19CommaDown"/>-->
            <!--<glyph glyph-code="E39A" smufl-name="accSagittal7v19CommaUp"/>-->
            <!--<glyph glyph-code="E39B" smufl-name="accSagittal7v19CommaDown"/>-->
            <!--<glyph glyph-code="E39C" smufl-name="accSagittal49SmallDiesisUp"/>-->
            <!--<glyph glyph-code="E39D" smufl-name="accSagittal49SmallDiesisDown"/>-->
            <!--<glyph glyph-code="E39E" smufl-name="accSagittal23SmallDiesisUp"/>-->
            <!--<glyph glyph-code="E39F" smufl-name="accSagittal23SmallDiesisDown"/>-->
            <!--<glyph glyph-code="E3A0" smufl-name="accSagittal5v13MediumDiesisUp"/>-->
            <!--<glyph glyph-code="E3A1" smufl-name="accSagittal5v13MediumDiesisDown"/>-->
            <!--<glyph glyph-code="E3A2" smufl-name="accSagittal11v19MediumDiesisUp"/>-->
            <!--<glyph glyph-code="E3A3" smufl-name="accSagittal11v19MediumDiesisDown"/>-->
            <!--<glyph glyph-code="E3A4" smufl-name="accSagittal49MediumDiesisUp"/>-->
            <!--<glyph glyph-code="E3A5" smufl-name="accSagittal49MediumDiesisDown"/>-->
            <!--<glyph glyph-code="E3A6" smufl-name="accSagittal5v49MediumDiesisUp"/>-->
            <!--<glyph glyph-code="E3A7" smufl-name="accSagittal5v49MediumDiesisDown"/>-->
            <!--<glyph glyph-code="E3A8" smufl-name="accSagittal49LargeDiesisUp"/>-->
            <!--<glyph glyph-code="E3A9" smufl-name="accSagittal49LargeDiesisDown"/>-->
            <!--<glyph glyph-code="E3AA" smufl-name="accSagittal11v19LargeDiesisUp"/>-->
            <!--<glyph glyph-code="E3AB" smufl-name="accSagittal11v19LargeDiesisDown"/>-->
            <!--<glyph glyph-code="E3AC" smufl-name="accSagittal5v13LargeDiesisUp"/>-->
            <!--<glyph glyph-code="E3AD" smufl-name="accSagittal5v13LargeDiesisDown"/>-->
            <range_end>U+E3AF</range_end>
            <range_start>U+E390</range_start>
        </prometheanSagittalExtensionHighPrecisionSingleShaftAccidentals>
        <prometheanSagittalExtensionHighPrecisionMultiShaftAccidentals>
            <description>Promethean Sagittal extension (high precision) multi-shaft accidentals</description>
            <!--<glyph glyph-code="E3B0" smufl-name="accSagittalSharp23SDown"/>-->
            <!--<glyph glyph-code="E3B1" smufl-name="accSagittalFlat23SUp"/>-->
            <!--<glyph glyph-code="E3B2" smufl-name="accSagittalSharp49SDown"/>-->
            <!--<glyph glyph-code="E3B3" smufl-name="accSagittalFlat49SUp"/>-->
            <!--<glyph glyph-code="E3B4" smufl-name="accSagittalSharp7v19CDown"/>-->
            <!--<glyph glyph-code="E3B5" smufl-name="accSagittalFlat7v19CUp"/>-->
            <!--<glyph glyph-code="E3B6" smufl-name="accSagittalSharp19CDown"/>-->
            <!--<glyph glyph-code="E3B7" smufl-name="accSagittalFlat19CUp"/>-->
            <!--<glyph glyph-code="E3B8" smufl-name="accSagittalSharp11v49CDown"/>-->
            <!--<glyph glyph-code="E3B9" smufl-name="accSagittalFlat11v49CUp"/>-->
            <!--<glyph glyph-code="E3BA" smufl-name="accSagittalSharp143CDown"/>-->
            <!--<glyph glyph-code="E3BB" smufl-name="accSagittalFlat143CUp"/>-->
            <!--<glyph glyph-code="E3BC" smufl-name="accSagittalSharp17kDown"/>-->
            <!--<glyph glyph-code="E3BD" smufl-name="accSagittalFlat17kUp"/>-->
            <!--<glyph glyph-code="E3BE" smufl-name="accSagittalSharp19sDown"/>-->
            <!--<glyph glyph-code="E3BF" smufl-name="accSagittalFlat19sUp"/>-->
            <!--<glyph glyph-code="E3C0" smufl-name="accSagittalSharp19sUp"/>-->
            <!--<glyph glyph-code="E3C1" smufl-name="accSagittalFlat19sDown"/>-->
            <!--<glyph glyph-code="E3C2" smufl-name="accSagittalSharp17kUp"/>-->
            <!--<glyph glyph-code="E3C3" smufl-name="accSagittalFlat17kDown"/>-->
            <!--<glyph glyph-code="E3C4" smufl-name="accSagittalSharp143CUp"/>-->
            <!--<glyph glyph-code="E3C5" smufl-name="accSagittalFlat143CDown"/>-->
            <!--<glyph glyph-code="E3C6" smufl-name="accSagittalSharp11v49CUp"/>-->
            <!--<glyph glyph-code="E3C7" smufl-name="accSagittalFlat11v49CDown"/>-->
            <!--<glyph glyph-code="E3C8" smufl-name="accSagittalSharp19CUp"/>-->
            <!--<glyph glyph-code="E3C9" smufl-name="accSagittalFlat19CDown"/>-->
            <!--<glyph glyph-code="E3CA" smufl-name="accSagittalSharp7v19CUp"/>-->
            <!--<glyph glyph-code="E3CB" smufl-name="accSagittalFlat7v19CDown"/>-->
            <!--<glyph glyph-code="E3CC" smufl-name="accSagittalSharp49SUp"/>-->
            <!--<glyph glyph-code="E3CD" smufl-name="accSagittalFlat49SDown"/>-->
            <!--<glyph glyph-code="E3CE" smufl-name="accSagittalSharp23SUp"/>-->
            <!--<glyph glyph-code="E3CF" smufl-name="accSagittalFlat23SDown"/>-->
            <!--<glyph glyph-code="E3D0" smufl-name="accSagittalSharp5v13MUp"/>-->
            <!--<glyph glyph-code="E3D1" smufl-name="accSagittalFlat5v13MDown"/>-->
            <!--<glyph glyph-code="E3D2" smufl-name="accSagittalSharp11v19MUp"/>-->
            <!--<glyph glyph-code="E3D3" smufl-name="accSagittalFlat11v19MDown"/>-->
            <!--<glyph glyph-code="E3D4" smufl-name="accSagittalSharp49MUp"/>-->
            <!--<glyph glyph-code="E3D5" smufl-name="accSagittalFlat49MDown"/>-->
            <!--<glyph glyph-code="E3D6" smufl-name="accSagittalSharp5v49MUp"/>-->
            <!--<glyph glyph-code="E3D7" smufl-name="accSagittalFlat5v49MDown"/>-->
            <!--<glyph glyph-code="E3D8" smufl-name="accSagittalSharp49LUp"/>-->
            <!--<glyph glyph-code="E3D9" smufl-name="accSagittalFlat49LDown"/>-->
            <!--<glyph glyph-code="E3DA" smufl-name="accSagittalSharp11v19LUp"/>-->
            <!--<glyph glyph-code="E3DB" smufl-name="accSagittalFlat11v19LDown"/>-->
            <!--<glyph glyph-code="E3DC" smufl-name="accSagittalSharp5v13LUp"/>-->
            <!--<glyph glyph-code="E3DD" smufl-name="accSagittalFlat5v13LDown"/>-->
            <!--<glyph glyph-code="E3DE" smufl-name="accSagittalUnused3"/>-->
            <!--<glyph glyph-code="E3DF" smufl-name="accSagittalUnused4"/>-->
            <!--<glyph glyph-code="E3E0" smufl-name="accSagittalDoubleSharp23SDown"/>-->
            <!--<glyph glyph-code="E3E1" smufl-name="accSagittalDoubleFlat23SUp"/>-->
            <!--<glyph glyph-code="E3E2" smufl-name="accSagittalDoubleSharp49SDown"/>-->
            <!--<glyph glyph-code="E3E3" smufl-name="accSagittalDoubleFlat49SUp"/>-->
            <!--<glyph glyph-code="E3E4" smufl-name="accSagittalDoubleSharp7v19CDown"/>-->
            <!--<glyph glyph-code="E3E5" smufl-name="accSagittalDoubleFlat7v19CUp"/>-->
            <!--<glyph glyph-code="E3E6" smufl-name="accSagittalDoubleSharp19CDown"/>-->
            <!--<glyph glyph-code="E3E7" smufl-name="accSagittalDoubleFlat19CUp"/>-->
            <!--<glyph glyph-code="E3E8" smufl-name="accSagittalDoubleSharp11v49CDown"/>-->
            <!--<glyph glyph-code="E3E9" smufl-name="accSagittalDoubleFlat11v49CUp"/>-->
            <!--<glyph glyph-code="E3EA" smufl-name="accSagittalDoubleSharp143CDown"/>-->
            <!--<glyph glyph-code="E3EB" smufl-name="accSagittalDoubleFlat143CUp"/>-->
            <!--<glyph glyph-code="E3EC" smufl-name="accSagittalDoubleSharp17kDown"/>-->
            <!--<glyph glyph-code="E3ED" smufl-name="accSagittalDoubleFlat17kUp"/>-->
            <!--<glyph glyph-code="E3EE" smufl-name="accSagittalDoubleSharp19sDown"/>-->
            <!--<glyph glyph-code="E3EF" smufl-name="accSagittalDoubleFlat19sUp"/>-->
            <range_end>U+E3EF</range_end>
            <range_start>U+E3B0</range_start>
        </prometheanSagittalExtensionHighPrecisionMultiShaftAccidentals>
        <herculeanSagittalExtensionVeryHighPrecisionAccidentalDiacritics>
            <description>Herculean Sagittal extension (very high precision) accidental diacritics</description>
            <!--<glyph glyph-code="E3F0" smufl-name="accSagittalShaftUp"/>-->
            <!--<glyph glyph-code="E3F1" smufl-name="accSagittalShaftDown"/>-->
            <!--<glyph glyph-code="E3F2" smufl-name="accSagittalAcute"/>-->
            <!--<glyph glyph-code="E3F3" smufl-name="accSagittalGrave"/>-->
            <range_end>U+E3FF</range_end>
            <range_start>U+E3F0</range_start>
        </herculeanSagittalExtensionVeryHighPrecisionAccidentalDiacritics>
        <olympianSagittalExtensionExtremePrecisionAccidentalDiacritics>
            <description>Olympian Sagittal extension (extreme precision) accidental diacritics</description>
            <range_end>U+E40F</range_end>
            <range_start>U+E400</range_start>
        </olympianSagittalExtensionExtremePrecisionAccidentalDiacritics>
        <magratheanSagittalExtensionInsanePrecisionAccidentalDiacritics>
            <description>Magrathean Sagittal extension (insane precision) accidental diacritics</description>
            <range_end>U+E41F</range_end>
            <range_start>U+E410</range_start>
        </magratheanSagittalExtensionInsanePrecisionAccidentalDiacritics>
        <wyschnegradskyAccidentals72Edo>
            <description>Wyschnegradsky accidentals (72-EDO)</description>
            <!--<glyph glyph-code="E420" smufl-name="accidentalWyschnegradsky1TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E421" smufl-name="accidentalWyschnegradsky2TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E422" smufl-name="accidentalWyschnegradsky3TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E423" smufl-name="accidentalWyschnegradsky4TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E424" smufl-name="accidentalWyschnegradsky5TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E425" smufl-name="accidentalWyschnegradsky6TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E426" smufl-name="accidentalWyschnegradsky7TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E427" smufl-name="accidentalWyschnegradsky8TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E428" smufl-name="accidentalWyschnegradsky9TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E429" smufl-name="accidentalWyschnegradsky10TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E42A" smufl-name="accidentalWyschnegradsky11TwelfthsSharp"/>-->
            <!--<glyph glyph-code="E42B" smufl-name="accidentalWyschnegradsky1TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E42C" smufl-name="accidentalWyschnegradsky2TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E42D" smufl-name="accidentalWyschnegradsky3TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E42E" smufl-name="accidentalWyschnegradsky4TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E42F" smufl-name="accidentalWyschnegradsky5TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E430" smufl-name="accidentalWyschnegradsky6TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E431" smufl-name="accidentalWyschnegradsky7TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E432" smufl-name="accidentalWyschnegradsky8TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E433" smufl-name="accidentalWyschnegradsky9TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E434" smufl-name="accidentalWyschnegradsky10TwelfthsFlat"/>-->
            <!--<glyph glyph-code="E435" smufl-name="accidentalWyschnegradsky11TwelfthsFlat"/>-->
            <range_end>U+E43F</range_end>
            <range_start>U+E420</range_start>
        </wyschnegradskyAccidentals72Edo>
        <arelEzgiUzdilekAeuAccidentals>
            <description>Arel-Ezgi-Uzdilek (AEU) accidentals</description>
            <!--<glyph glyph-code="E440" smufl-name="accidentalBuyukMucennebFlat"/>-->
            <!--<glyph glyph-code="E441" smufl-name="accidentalKucukMucennebFlat"/>-->
            <!--<glyph glyph-code="E442" smufl-name="accidentalBakiyeFlat"/>-->
            <!--<glyph glyph-code="E443" smufl-name="accidentalKomaFlat"/>-->
            <!--<glyph glyph-code="E444" smufl-name="accidentalKomaSharp"/>-->
            <!--<glyph glyph-code="E445" smufl-name="accidentalBakiyeSharp"/>-->
            <!--<glyph glyph-code="E446" smufl-name="accidentalKucukMucennebSharp"/>-->
            <!--<glyph glyph-code="E447" smufl-name="accidentalBuyukMucennebSharp"/>-->
            <range_end>U+E44F</range_end>
            <range_start>U+E440</range_start>
        </arelEzgiUzdilekAeuAccidentals>
        <turkishFolkMusicAccidentals>
            <description>Turkish folk music accidentals</description>
            <!--<glyph glyph-code="E450" smufl-name="accidental1CommaSharp"/>-->
            <!--<glyph glyph-code="E451" smufl-name="accidental2CommaSharp"/>-->
            <!--<glyph glyph-code="E452" smufl-name="accidental3CommaSharp"/>-->
            <!--<glyph glyph-code="E453" smufl-name="accidental5CommaSharp"/>-->
            <!--<glyph glyph-code="E454" smufl-name="accidental1CommaFlat"/>-->
            <!--<glyph glyph-code="E455" smufl-name="accidental2CommaFlat"/>-->
            <!--<glyph glyph-code="E456" smufl-name="accidental3CommaFlat"/>-->
            <!--<glyph glyph-code="E457" smufl-name="accidental4CommaFlat"/>-->
            <range_end>U+E45F</range_end>
            <range_start>U+E450</range_start>
        </turkishFolkMusicAccidentals>
        <persianAccidentals>
            <description>Persian accidentals</description>
            <!--<glyph glyph-code="E460" smufl-name="accidentalKoron"/>-->
            <!--<glyph glyph-code="E461" smufl-name="accidentalSori"/>-->
            <range_end>U+E46F</range_end>
            <range_start>U+E460</range_start>
        </persianAccidentals>
        <otherAccidentals>
            <description>Other accidentals</description>
            <!--<glyph glyph-code="E470" smufl-name="accidentalXenakisOneThirdToneSharp"/>-->
            <!--<glyph glyph-code="E471" smufl-name="accidentalXenakisTwoThirdTonesSharp"/>-->
            <!--<glyph glyph-code="E472" smufl-name="accidentalQuarterToneSharpBusotti"/>-->
            <!--<glyph glyph-code="E473" smufl-name="accidentalSharpOneHorizontalStroke"/>-->
            <!--<glyph glyph-code="E474" smufl-name="accidentalThreeQuarterTonesSharpBusotti"/>-->
            <!--<glyph glyph-code="E475" smufl-name="accidentalQuarterToneSharpWiggle"/>-->
            <!--<glyph glyph-code="E476" smufl-name="accidentalTavenerSharp"/>-->
            <!--<glyph glyph-code="E477" smufl-name="accidentalTavenerFlat"/>-->
            <!--<glyph glyph-code="E478" smufl-name="accidentalQuarterToneFlatPenderecki"/>-->
            <!--<glyph glyph-code="E479" smufl-name="accidentalCommaSlashUp"/>-->
            <!--<glyph glyph-code="E47A" smufl-name="accidentalCommaSlashDown"/>-->
            <!--<glyph glyph-code="E47B" smufl-name="accidentalWilsonPlus"/>-->
            <!--<glyph glyph-code="E47C" smufl-name="accidentalWilsonMinus"/>-->
            <!--<glyph glyph-code="E47D" smufl-name="accidentalLargeDoubleSharp"/>-->
            <!--<glyph glyph-code="E47E" smufl-name="accidentalQuarterToneSharp4"/>-->
            <!--<glyph glyph-code="E47F" smufl-name="accidentalQuarterToneFlat4"/>-->
            <!--<glyph glyph-code="E480" smufl-name="accidentalQuarterToneFlatFilledReversed"/>-->
            <!--<glyph glyph-code="E481" smufl-name="accidentalSharpReversed"/>-->
            <!--<glyph glyph-code="E482" smufl-name="accidentalNaturalReversed"/>-->
            <!--<glyph glyph-code="E483" smufl-name="accidentalDoubleFlatReversed"/>-->
            <!--<glyph glyph-code="E484" smufl-name="accidentalFlatTurned"/>-->
            <!--<glyph glyph-code="E485" smufl-name="accidentalDoubleFlatTurned"/>-->
            <!--<glyph glyph-code="E486" smufl-name="accidentalThreeQuarterTonesFlatGrisey"/>-->
            <!--<glyph glyph-code="E487" smufl-name="accidentalThreeQuarterTonesFlatTartini"/>-->
            <!--<glyph glyph-code="E488" smufl-name="accidentalQuarterToneFlatVanBlankenburg"/>-->
            <!--<glyph glyph-code="E489" smufl-name="accidentalThreeQuarterTonesFlatCouper"/>-->
            <!--<glyph glyph-code="E48A" smufl-name="accidentalOneThirdToneSharpFerneyhough"/>-->
            <!--<glyph glyph-code="E48B" smufl-name="accidentalOneThirdToneFlatFerneyhough"/>-->
            <!--<glyph glyph-code="E48C" smufl-name="accidentalTwoThirdTonesSharpFerneyhough"/>-->
            <!--<glyph glyph-code="E48D" smufl-name="accidentalTwoThirdTonesFlatFerneyhough"/>-->
            <range_end>U+E49F</range_end>
            <range_start>U+E470</range_start>
        </otherAccidentals>
        <articulation>
            <description>Articulation</description>
            <glyph glyph-code="E4A0" smufl-name="articAccentAbove"/>-->
            <glyph glyph-code="E4A1" smufl-name="articAccentBelow"/>-->
            <glyph glyph-code="E4A2" smufl-name="articStaccatoAbove"/>
            <glyph glyph-code="E4A3" smufl-name="articStaccatoBelow"/>
            <glyph glyph-code="E4A4" smufl-name="articTenutoAbove"/>
            <glyph glyph-code="E4A5" smufl-name="articTenutoBelow"/>
            <glyph glyph-code="E4A6" smufl-name="articStaccatissimoAbove"/>
            <glyph glyph-code="E4A7" smufl-name="articStaccatissimoBelow"/>
            <glyph glyph-code="E4A8" smufl-name="articStaccatissimoWedgeAbove"/>
            <glyph glyph-code="E4A9" smufl-name="articStaccatissimoWedgeBelow"/>
            <glyph glyph-code="E4AA" smufl-name="articStaccatissimoStrokeAbove"/>
            <glyph glyph-code="E4AB" smufl-name="articStaccatissimoStrokeBelow"/>
            <glyph glyph-code="E4AC" smufl-name="articMarcatoAbove"/>
            <glyph glyph-code="E4AD" smufl-name="articMarcatoBelow"/>
            <glyph glyph-code="E4AE" smufl-name="articMarcatoStaccatoAbove"/>
            <glyph glyph-code="E4AF" smufl-name="articMarcatoStaccatoBelow"/>
            <glyph glyph-code="E4B0" smufl-name="articAccentStaccatoAbove"/>
            <glyph glyph-code="E4B1" smufl-name="articAccentStaccatoBelow"/>
            <glyph glyph-code="E4B2" smufl-name="articTenutoStaccatoAbove"/>
            <glyph glyph-code="E4B3" smufl-name="articTenutoStaccatoBelow"/>
            <glyph glyph-code="E4B4" smufl-name="articTenutoAccentAbove"/>
            <glyph glyph-code="E4B5" smufl-name="articTenutoAccentBelow"/>
            <glyph glyph-code="E4B6" smufl-name="articStressAbove"/>
            <glyph glyph-code="E4B7" smufl-name="articStressBelow"/>
            <glyph glyph-code="E4B8" smufl-name="articUnstressAbove"/>
            <glyph glyph-code="E4B9" smufl-name="articUnstressBelow"/>
            <glyph glyph-code="E4BA" smufl-name="articLaissezVibrerAbove"/>
            <glyph glyph-code="E4BB" smufl-name="articLaissezVibrerBelow"/>
            <glyph glyph-code="E4BC" smufl-name="articMarcatoTenutoAbove"/>
            <glyph glyph-code="E4BD" smufl-name="articMarcatoTenutoBelow"/>
            <range_end>U+E4BF</range_end>
            <range_start>U+E4A0</range_start>
        </articulation>
        <holdsAndPauses>
            <description>Holds and pauses</description>
            <glyph glyph-code="E4C0" smufl-name="fermataAbove"/>
            <glyph glyph-code="E4C1" smufl-name="fermataBelow"/>
            <glyph glyph-code="E4C2" smufl-name="fermataVeryShortAbove"/>
            <glyph glyph-code="E4C3" smufl-name="fermataVeryShortBelow"/>
            <glyph glyph-code="E4C4" smufl-name="fermataShortAbove"/>
            <glyph glyph-code="E4C5" smufl-name="fermataShortBelow"/>
            <glyph glyph-code="E4C6" smufl-name="fermataLongAbove"/>
            <glyph glyph-code="E4C7" smufl-name="fermataLongBelow"/>
            <glyph glyph-code="E4C8" smufl-name="fermataVeryLongAbove"/>
            <glyph glyph-code="E4C9" smufl-name="fermataVeryLongBelow"/>
            <!--<glyph glyph-code="E4CA" smufl-name="fermataLongHenzeAbove"/>-->
            <!--<glyph glyph-code="E4CB" smufl-name="fermataLongHenzeBelow"/>-->
            <!--<glyph glyph-code="E4CC" smufl-name="fermataShortHenzeAbove"/>-->
            <!--<glyph glyph-code="E4CD" smufl-name="fermataShortHenzeBelow"/>-->
            <glyph glyph-code="E4CE" smufl-name="breathMarkComma"/>
            <!--<glyph glyph-code="E4CF" smufl-name="breathMarkTick"/>-->
            <!--<glyph glyph-code="E4D0" smufl-name="breathMarkUpbow"/>-->
            <!--<glyph glyph-code="E4D1" smufl-name="caesura"/>-->
            <!--<glyph glyph-code="E4D2" smufl-name="caesuraThick"/>-->
            <!--<glyph glyph-code="E4D3" smufl-name="caesuraShort"/>-->
            <!--<glyph glyph-code="E4D4" smufl-name="caesuraCurved"/>-->
            <!--<glyph glyph-code="E4D5" smufl-name="breathMarkSalzedo"/>-->
            <!--<glyph glyph-code="E4D6" smufl-name="curlewSign"/>-->
            <range_end>U+E4DF</range_end>
            <range_start>U+E4C0</range_start>
        </holdsAndPauses>
        <rests>
            <description>Rests</description>
            <!--<glyph glyph-code="E4E0" smufl-name="restMaxima"/>-->
            <!--<glyph glyph-code="E4E1" smufl-name="restLonga"/>-->
            <!--<glyph glyph-code="E4E2" smufl-name="restDoubleWhole"/>-->
            <!--<glyph glyph-code="E4E3" smufl-name="restWhole"/>-->
            <!--<glyph glyph-code="E4E4" smufl-name="restHalf"/>-->
            <glyph glyph-code="E4E5" smufl-name="restQuarter"/>
            <glyph glyph-code="E4E6" smufl-name="rest8th"/>
            <glyph glyph-code="E4E7" smufl-name="rest16th"/>
            <glyph glyph-code="E4E8" smufl-name="rest32nd"/>
            <glyph glyph-code="E4E9" smufl-name="rest64th"/>
            <glyph glyph-code="E4EA" smufl-name="rest128th"/>
            <glyph glyph-code="E4EB" smufl-name="rest256th"/>
            <glyph glyph-code="E4EC" smufl-name="rest512th"/>
            <glyph glyph-code="E4ED" smufl-name="rest1024th"/>
            <!--<glyph glyph-code="E4EE" smufl-name="restHBar"/>-->
            <!--<glyph glyph-code="E4EF" smufl-name="restHBarLeft"/>-->
            <!--<glyph glyph-code="E4F0" smufl-name="restHBarMiddle"/>-->
            <!--<glyph glyph-code="E4F1" smufl-name="restHBarRight"/>-->
            <!--<glyph glyph-code="E4F2" smufl-name="restQuarterOld"/>-->
            <!--<glyph glyph-code="E4F3" smufl-name="restDoubleWholeLegerLine"/>-->
            <!--<glyph glyph-code="E4F4" smufl-name="restWholeLegerLine"/>-->
            <!--<glyph glyph-code="E4F5" smufl-name="restHalfLegerLine"/>-->
            <range_end>U+E4FF</range_end>
            <range_start>U+E4E0</range_start>
        </rests>
        <barRepeats>
            <description>Bar repeats</description>
            <glyph glyph-code="E500" smufl-name="repeat1Bar"/>
            <glyph glyph-code="E501" smufl-name="repeat2Bars"/>
            <glyph glyph-code="E502" smufl-name="repeat4Bars"/>
            <range_end>U+E50F</range_end>
            <range_start>U+E500</range_start>
        </barRepeats>
        <octaves>
            <description>Octaves</description>
            <glyph glyph-code="E510" smufl-name="ottava"/>
            <glyph glyph-code="E511" smufl-name="ottavaAlta"/>
            <glyph glyph-code="E512" smufl-name="ottavaBassa"/>
            <glyph glyph-code="E513" smufl-name="ottavaBassaBa"/>
            <glyph glyph-code="E514" smufl-name="quindicesima"/>
            <glyph glyph-code="E515" smufl-name="quindicesimaAlta"/>
            <glyph glyph-code="E516" smufl-name="quindicesimaBassa"/>
            <glyph glyph-code="E517" smufl-name="ventiduesima"/>
            <glyph glyph-code="E518" smufl-name="ventiduesimaAlta"/>
            <glyph glyph-code="E519" smufl-name="ventiduesimaBassa"/>
            <!--<glyph glyph-code="E51A" smufl-name="octaveParensLeft"/>-->
            <!--<glyph glyph-code="E51B" smufl-name="octaveParensRight"/>-->
            <range_end>U+E51F</range_end>
            <range_start>U+E510</range_start>
        </octaves>
        <dynamics>
            <description>Dynamics</description>
            <glyph glyph-code="E520" smufl-name="dynamicPiano"/>
            <glyph glyph-code="E521" smufl-name="dynamicMezzo"/>
            <glyph glyph-code="E522" smufl-name="dynamicForte"/>
            <glyph glyph-code="E523" smufl-name="dynamicRinforzando"/>
            <glyph glyph-code="E524" smufl-name="dynamicSforzando"/>
            <glyph glyph-code="E525" smufl-name="dynamicZ"/>
            <glyph glyph-code="E526" smufl-name="dynamicNiente"/>
            <glyph glyph-code="E527" smufl-name="dynamicPPPPPP"/>
            <glyph glyph-code="E528" smufl-name="dynamicPPPPP"/>
            <glyph glyph-code="E529" smufl-name="dynamicPPPP"/>
            <glyph glyph-code="E52A" smufl-name="dynamicPPP"/>
            <glyph glyph-code="E52B" smufl-name="dynamicPP"/>
            <glyph glyph-code="E52C" smufl-name="dynamicMP"/>
            <glyph glyph-code="E52D" smufl-name="dynamicMF"/>
            <glyph glyph-code="E52E" smufl-name="dynamicPF"/>
            <glyph glyph-code="E52F" smufl-name="dynamicFF"/>
            <glyph glyph-code="E530" smufl-name="dynamicFFF"/>
            <glyph glyph-code="E531" smufl-name="dynamicFFFF"/>
            <glyph glyph-code="E532" smufl-name="dynamicFFFFF"/>
            <glyph glyph-code="E533" smufl-name="dynamicFFFFFF"/>
            <glyph glyph-code="E534" smufl-name="dynamicFortePiano"/>
            <glyph glyph-code="E535" smufl-name="dynamicForzando"/>
            <glyph glyph-code="E536" smufl-name="dynamicSforzando1"/>
            <glyph glyph-code="E537" smufl-name="dynamicSforzandoPiano"/>
            <glyph glyph-code="E538" smufl-name="dynamicSforzandoPianissimo"/>
            <glyph glyph-code="E539" smufl-name="dynamicSforzato"/>
            <glyph glyph-code="E53A" smufl-name="dynamicSforzatoPiano"/>
            <glyph glyph-code="E53B" smufl-name="dynamicSforzatoFF"/>
            <glyph glyph-code="E53C" smufl-name="dynamicRinforzando1"/>
            <glyph glyph-code="E53D" smufl-name="dynamicRinforzando2"/>
            <!--<glyph glyph-code="E53E" smufl-name="dynamicCrescendoHairpin"/>-->
            <!--<glyph glyph-code="E53F" smufl-name="dynamicDiminuendoHairpin"/>-->
            <!--<glyph glyph-code="E540" smufl-name="dynamicMessaDiVoce"/>-->
            <!--<glyph glyph-code="E541" smufl-name="dynamicNienteForHairpin"/>-->
            <range_end>U+E54F</range_end>
            <range_start>U+E520</range_start>
        </dynamics>
        <lyrics>
            <description>Lyrics</description>
            <!--<glyph glyph-code="E550" smufl-name="lyricsElisionNarrow"/>-->
            <!--<glyph glyph-code="E551" smufl-name="lyricsElision"/>-->
            <!--<glyph glyph-code="E552" smufl-name="lyricsElisionWide"/>-->
            <!--<glyph glyph-code="E553" smufl-name="lyricsHyphenBaseline"/>-->
            <!--<glyph glyph-code="E554" smufl-name="lyricsHyphenBaselineNonBreaking"/>-->
            <range_end>U+E55F</range_end>
            <range_start>U+E550</range_start>
        </lyrics>
        <commonOrnaments>
            <description>Common ornaments</description>
            <!--<glyph glyph-code="E560" smufl-name="graceNoteAcciaccaturaStemUp"/>-->
            <!--<glyph glyph-code="E561" smufl-name="graceNoteAcciaccaturaStemDown"/>-->
            <!--<glyph glyph-code="E562" smufl-name="graceNoteAppoggiaturaStemUp"/>-->
            <!--<glyph glyph-code="E563" smufl-name="graceNoteAppoggiaturaStemDown"/>-->
            <!--<glyph glyph-code="E564" smufl-name="graceNoteSlashStemUp"/>-->
            <!--<glyph glyph-code="E565" smufl-name="graceNoteSlashStemDown"/>-->
            <glyph glyph-code="E566" smufl-name="ornamentTrill"/>
            <glyph glyph-code="E567" smufl-name="ornamentTurn"/>
            <glyph glyph-code="E568" smufl-name="ornamentTurnInverted"/>
            <!--<glyph glyph-code="E569" smufl-name="ornamentTurnSlash"/>-->
            <glyph glyph-code="E56A" smufl-name="ornamentTurnUp"/>
            <glyph glyph-code="E56B" smufl-name="ornamentTurnUpS"/>
            <glyph glyph-code="E56C" smufl-name="ornamentMordent"/>
            <glyph glyph-code="E56D" smufl-name="ornamentMordentInverted"/>
            <glyph glyph-code="E56E" smufl-name="ornamentTremblement"/>
            <!--<glyph glyph-code="E56F" smufl-name="ornamentHaydn"/>-->
            <range_end>U+E56F</range_end>
            <range_start>U+E560</range_start>
        </commonOrnaments>
        <otherBaroqueOrnaments>
            <description>Other baroque ornaments</description>
            <!--<glyph glyph-code="E570" smufl-name="ornamentPortDeVoixV"/>-->
            <!--<glyph glyph-code="E571" smufl-name="ornamentRightFacingHalfCircle"/>-->
            <!--<glyph glyph-code="E572" smufl-name="ornamentLeftFacingHalfCircle"/>-->
            <!--<glyph glyph-code="E573" smufl-name="ornamentRightFacingHook"/>-->
            <!--<glyph glyph-code="E574" smufl-name="ornamentLeftFacingHook"/>-->
            <!--<glyph glyph-code="E575" smufl-name="ornamentHookBeforeNote"/>-->
            <!--<glyph glyph-code="E576" smufl-name="ornamentHookAfterNote"/>-->
            <!--<glyph glyph-code="E577" smufl-name="ornamentUpCurve"/>-->
            <!--<glyph glyph-code="E578" smufl-name="ornamentDownCurve"/>-->
            <!--<glyph glyph-code="E579" smufl-name="ornamentShortObliqueLineBeforeNote"/>-->
            <!--<glyph glyph-code="E57A" smufl-name="ornamentShortObliqueLineAfterNote"/>-->
            <!--<glyph glyph-code="E57B" smufl-name="ornamentObliqueLineBeforeNote"/>-->
            <!--<glyph glyph-code="E57C" smufl-name="ornamentObliqueLineAfterNote"/>-->
            <!--<glyph glyph-code="E57D" smufl-name="ornamentDoubleObliqueLinesBeforeNote"/>-->
            <!--<glyph glyph-code="E57E" smufl-name="ornamentDoubleObliqueLinesAfterNote"/>-->
            <!--<glyph glyph-code="E57F" smufl-name="ornamentObliqueLineHorizBeforeNote"/>-->
            <!--<glyph glyph-code="E580" smufl-name="ornamentObliqueLineHorizAfterNote"/>-->
            <!--<glyph glyph-code="E581" smufl-name="ornamentComma"/>-->
            <!--<glyph glyph-code="E582" smufl-name="ornamentShake3"/>-->
            <!--<glyph glyph-code="E583" smufl-name="ornamentVerticalLine"/>-->
            <!--<glyph glyph-code="E584" smufl-name="ornamentShakeMuffat1"/>-->
            <!--<glyph glyph-code="E585" smufl-name="glissandoUp"/>-->
            <!--<glyph glyph-code="E586" smufl-name="glissandoDown"/>-->
            <!--<glyph glyph-code="E587" smufl-name="ornamentSchleifer"/>-->
            <!--<glyph glyph-code="E588" smufl-name="ornamentPinceCouperin"/>-->
            <!--<glyph glyph-code="E589" smufl-name="ornamentTremblementCouperin"/>-->
            <range_end>U+E58F</range_end>
            <range_start>U+E570</range_start>
        </otherBaroqueOrnaments>
        <combiningStrokesForTrillsAndMordents>
            <description>Combining strokes for trills and mordents</description>
            <!--<glyph glyph-code="E590" smufl-name="ornamentTopLeftConcaveStroke"/>-->
            <!--<glyph glyph-code="E591" smufl-name="ornamentTopLeftConvexStroke"/>-->
            <!--<glyph glyph-code="E592" smufl-name="ornamentHighLeftConcaveStroke"/>-->
            <!--<glyph glyph-code="E593" smufl-name="ornamentHighLeftConvexStroke"/>-->
            <!--<glyph glyph-code="E594" smufl-name="ornamentLeftVerticalStroke"/>-->
            <!--<glyph glyph-code="E595" smufl-name="ornamentLeftVerticalStrokeWithCross"/>-->
            <!--<glyph glyph-code="E596" smufl-name="ornamentLeftShakeT"/>-->
            <!--<glyph glyph-code="E597" smufl-name="ornamentLeftPlus"/>-->
            <!--<glyph glyph-code="E598" smufl-name="ornamentLowLeftConcaveStroke"/>-->
            <!--<glyph glyph-code="E599" smufl-name="ornamentLowLeftConvexStroke"/>-->
            <!--<glyph glyph-code="E59A" smufl-name="ornamentBottomLeftConcaveStroke"/>-->
            <!--<glyph glyph-code="E59B" smufl-name="ornamentBottomLeftConcaveStrokeLarge"/>-->
            <!--<glyph glyph-code="E59C" smufl-name="ornamentBottomLeftConvexStroke"/>-->
            <glyph glyph-code="E59D" smufl-name="ornamentZigZagLineNoRightEnd"/>
            <glyph glyph-code="E59E" smufl-name="ornamentZigZagLineWithRightEnd"/>
            <!--<glyph glyph-code="E59F" smufl-name="ornamentMiddleVerticalStroke"/>-->
            <!--<glyph glyph-code="E5A0" smufl-name="ornamentTopRightConcaveStroke"/>-->
            <!--<glyph glyph-code="E5A1" smufl-name="ornamentTopRightConvexStroke"/>-->
            <!--<glyph glyph-code="E5A2" smufl-name="ornamentHighRightConcaveStroke"/>-->
            <!--<glyph glyph-code="E5A3" smufl-name="ornamentHighRightConvexStroke"/>-->
            <!--<glyph glyph-code="E5A4" smufl-name="ornamentRightVerticalStroke"/>-->
            <!--<glyph glyph-code="E5A5" smufl-name="ornamentLowRightConcaveStroke"/>-->
            <!--<glyph glyph-code="E5A6" smufl-name="ornamentLowRightConvexStroke"/>-->
            <!--<glyph glyph-code="E5A7" smufl-name="ornamentBottomRightConcaveStroke"/>-->
            <!--<glyph glyph-code="E5A8" smufl-name="ornamentBottomRightConvexStroke"/>-->
            <range_end>U+E5AF</range_end>
            <range_start>U+E590</range_start>
        </combiningStrokesForTrillsAndMordents>
        <precomposedTrillsAndMordents>
            <description>Precomposed trills and mordents</description>
            <!--<glyph glyph-code="E5B0" smufl-name="ornamentPrecompSlide"/>-->
            <!--<glyph glyph-code="E5B1" smufl-name="ornamentPrecompDescendingSlide"/>-->
            <!--<glyph glyph-code="E5B2" smufl-name="ornamentPrecompAppoggTrill"/>-->
            <!--<glyph glyph-code="E5B3" smufl-name="ornamentPrecompAppoggTrillSuffix"/>-->
            <!--<glyph glyph-code="E5B4" smufl-name="ornamentPrecompTurnTrillDAnglebert"/>-->
            <!--<glyph glyph-code="E5B5" smufl-name="ornamentPrecompSlideTrillDAnglebert"/>-->
            <!--<glyph glyph-code="E5B6" smufl-name="ornamentPrecompSlideTrillMarpurg"/>-->
            <!--<glyph glyph-code="E5B7" smufl-name="ornamentPrecompTurnTrillBach"/>-->
            <!--<glyph glyph-code="E5B8" smufl-name="ornamentPrecompSlideTrillBach"/>-->
            <!--<glyph glyph-code="E5B9" smufl-name="ornamentPrecompSlideTrillMuffat"/>-->
            <!--<glyph glyph-code="E5BA" smufl-name="ornamentPrecompSlideTrillSuffixMuffat"/>-->
            <!--<glyph glyph-code="E5BB" smufl-name="ornamentPrecompTrillSuffixDandrieu"/>-->
            <!--<glyph glyph-code="E5BC" smufl-name="ornamentPrecompPortDeVoixMordent"/>-->
            <!--<glyph glyph-code="E5BD" smufl-name="ornamentPrecompTrillWithMordent"/>-->
            <!--<glyph glyph-code="E5BE" smufl-name="ornamentPrecompCadence"/>-->
            <!--<glyph glyph-code="E5BF" smufl-name="ornamentPrecompCadenceWithTurn "/>-->
            <!--<glyph glyph-code="E5C0" smufl-name="ornamentPrecompDoubleCadenceLowerPrefix"/>-->
            <!--<glyph glyph-code="E5C1" smufl-name="ornamentPrecompCadenceUpperPrefix "/>-->
            <!--<glyph glyph-code="E5C2" smufl-name="ornamentPrecompCadenceUpperPrefixTurn"/>-->
            <!--<glyph glyph-code="E5C3" smufl-name="ornamentPrecompDoubleCadenceUpperPrefix "/>-->
            <!--<glyph glyph-code="E5C4"
                smufl-name="ornamentPrecompDoubleCadenceUpperPrefixTurn"/>-->
            <!--<glyph glyph-code="E5C5" smufl-name="ornamentPrecompMordentRelease"/>-->
            <!--<glyph glyph-code="E5C6" smufl-name="ornamentPrecompMordentUpperPrefix"/>-->
            <!--<glyph glyph-code="E5C7"
                smufl-name="ornamentPrecompInvertedMordentUpperPrefix"/>-->
            <!--<glyph glyph-code="E5C8" smufl-name="ornamentPrecompTrillLowerSuffix"/>-->
            <range_end>U+E5CF</range_end>
            <range_start>U+E5B0</range_start>
        </precomposedTrillsAndMordents>
        <brassTechniques>
            <description>Brass techniques</description>
            <!--<glyph glyph-code="E5D0" smufl-name="brassScoop"/>-->
            <!--<glyph glyph-code="E5D1" smufl-name="brassLiftShort"/>-->
            <!--<glyph glyph-code="E5D2" smufl-name="brassLiftMedium"/>-->
            <!--<glyph glyph-code="E5D3" smufl-name="brassLiftLong"/>-->
            <!--<glyph glyph-code="E5D4" smufl-name="brassDoitShort"/>-->
            <!--<glyph glyph-code="E5D5" smufl-name="brassDoitMedium"/>-->
            <!--<glyph glyph-code="E5D6" smufl-name="brassDoitLong"/>-->
            <!--<glyph glyph-code="E5D7" smufl-name="brassFallLipShort"/>-->
            <!--<glyph glyph-code="E5D8" smufl-name="brassFallLipMedium"/>-->
            <!--<glyph glyph-code="E5D9" smufl-name="brassFallLipLong"/>-->
            <!--<glyph glyph-code="E5DA" smufl-name="brassFallSmoothShort"/>-->
            <!--<glyph glyph-code="E5DB" smufl-name="brassFallSmoothMedium"/>-->
            <!--<glyph glyph-code="E5DC" smufl-name="brassFallSmoothLong"/>-->
            <!--<glyph glyph-code="E5DD" smufl-name="brassFallRoughShort"/>-->
            <!--<glyph glyph-code="E5DE" smufl-name="brassFallRoughMedium"/>-->
            <!--<glyph glyph-code="E5DF" smufl-name="brassFallRoughLong"/>-->
            <!--<glyph glyph-code="E5E0" smufl-name="brassPlop"/>-->
            <!--<glyph glyph-code="E5E1" smufl-name="brassFlip"/>-->
            <!--<glyph glyph-code="E5E2" smufl-name="brassSmear"/>-->
            <!--<glyph glyph-code="E5E3" smufl-name="brassBend"/>-->
            <!--<glyph glyph-code="E5E4" smufl-name="brassJazzTurn"/>-->
            <!--<glyph glyph-code="E5E5" smufl-name="brassMuteClosed"/>-->
            <!--<glyph glyph-code="E5E6" smufl-name="brassMuteHalfClosed"/>-->
            <!--<glyph glyph-code="E5E7" smufl-name="brassMuteOpen"/>-->
            <!--<glyph glyph-code="E5E8" smufl-name="brassHarmonMuteClosed"/>-->
            <!--<glyph glyph-code="E5E9" smufl-name="brassHarmonMuteStemHalfLeft"/>-->
            <!--<glyph glyph-code="E5EA" smufl-name="brassHarmonMuteStemHalfRight"/>-->
            <!--<glyph glyph-code="E5EB" smufl-name="brassHarmonMuteStemOpen"/>-->
            <range_end>U+E5EF</range_end>
            <range_start>U+E5D0</range_start>
        </brassTechniques>
        <windTechniques>
            <description>Wind techniques</description>
            <!--<glyph glyph-code="E5F0" smufl-name="doubleTongueAbove"/>-->
            <!--<glyph glyph-code="E5F1" smufl-name="doubleTongueBelow"/>-->
            <!--<glyph glyph-code="E5F2" smufl-name="tripleTongueAbove"/>-->
            <!--<glyph glyph-code="E5F3" smufl-name="tripleTongueBelow"/>-->
            <!--<glyph glyph-code="E5F4" smufl-name="windClosedHole"/>-->
            <!--<glyph glyph-code="E5F5" smufl-name="windThreeQuartersClosedHole"/>-->
            <!--<glyph glyph-code="E5F6" smufl-name="windHalfClosedHole1"/>-->
            <!--<glyph glyph-code="E5F7" smufl-name="windHalfClosedHole2"/>-->
            <!--<glyph glyph-code="E5F8" smufl-name="windHalfClosedHole3"/>-->
            <!--<glyph glyph-code="E5F9" smufl-name="windOpenHole"/>-->
            <!--<glyph glyph-code="E5FA" smufl-name="windTrillKey"/>-->
            <!--<glyph glyph-code="E5FB" smufl-name="windFlatEmbouchure"/>-->
            <!--<glyph glyph-code="E5FC" smufl-name="windSharpEmbouchure"/>-->
            <!--<glyph glyph-code="E5FD" smufl-name="windRelaxedEmbouchure"/>-->
            <!--<glyph glyph-code="E5FE" smufl-name="windLessRelaxedEmbouchure"/>-->
            <!--<glyph glyph-code="E5FF" smufl-name="windTightEmbouchure"/>-->
            <!--<glyph glyph-code="E600" smufl-name="windLessTightEmbouchure"/>-->
            <!--<glyph glyph-code="E601" smufl-name="windVeryTightEmbouchure"/>-->
            <!--<glyph glyph-code="E602" smufl-name="windWeakAirPressure"/>-->
            <!--<glyph glyph-code="E603" smufl-name="windStrongAirPressure"/>-->
            <!--<glyph glyph-code="E604" smufl-name="windReedPositionNormal"/>-->
            <!--<glyph glyph-code="E605" smufl-name="windReedPositionOut"/>-->
            <!--<glyph glyph-code="E606" smufl-name="windReedPositionIn"/>-->
            <!--<glyph glyph-code="E607" smufl-name="windMultiphonicsBlackStem"/>-->
            <!--<glyph glyph-code="E608" smufl-name="windMultiphonicsWhiteStem"/>-->
            <!--<glyph glyph-code="E609" smufl-name="windMultiphonicsBlackWhiteStem"/>-->
            <range_end>U+E60F</range_end>
            <range_start>U+E5F0</range_start>
        </windTechniques>
        <stringTechniques>
            <description>String techniques</description>
            <glyph glyph-code="E610" smufl-name="stringsDownBow"/>
            <glyph glyph-code="E611" smufl-name="stringsDownBowTurned"/>
            <glyph glyph-code="E612" smufl-name="stringsUpBow"/>
            <glyph glyph-code="E613" smufl-name="stringsUpBowTurned"/>
            <glyph glyph-code="E614" smufl-name="stringsHarmonic"/>
            <!--<glyph glyph-code="E615" smufl-name="stringsHalfHarmonic"/>-->
            <!--<glyph glyph-code="E616" smufl-name="stringsMuteOn"/>-->
            <!--<glyph glyph-code="E617" smufl-name="stringsMuteOff"/>-->
            <!--<glyph glyph-code="E618" smufl-name="stringsBowBehindBridge"/>-->
            <!--<glyph glyph-code="E619" smufl-name="stringsBowOnBridge"/>-->
            <!--<glyph glyph-code="E61A" smufl-name="stringsBowOnTailpiece"/>-->
            <!--<glyph glyph-code="E61B" smufl-name="stringsOverpressureDownBow"/>-->
            <!--<glyph glyph-code="E61C" smufl-name="stringsOverpressureUpBow"/>-->
            <!--<glyph glyph-code="E61D" smufl-name="stringsOverpressurePossibileDownBow"/>-->
            <!--<glyph glyph-code="E61E" smufl-name="stringsOverpressurePossibileUpBow"/>-->
            <!--<glyph glyph-code="E61F" smufl-name="stringsOverpressureNoDirection"/>-->
            <!--<glyph glyph-code="E620" smufl-name="stringsJeteAbove"/>-->
            <!--<glyph glyph-code="E621" smufl-name="stringsJeteBelow"/>-->
            <!--<glyph glyph-code="E622" smufl-name="stringsFouette"/>-->
            <!--<glyph glyph-code="E623" smufl-name="stringsVibratoPulse"/>-->
            <!--<glyph glyph-code="E624" smufl-name="stringsThumbPosition"/>-->
            <!--<glyph glyph-code="E625" smufl-name="stringsThumbPositionTurned"/>-->
            <!--<glyph glyph-code="E626" smufl-name="stringsChangeBowDirection"/>-->
            <range_end>U+E62F</range_end>
            <range_start>U+E610</range_start>
        </stringTechniques>
        <pluckedTechniques>
            <description>Plucked techniques</description>
            <!--<glyph glyph-code="E630" smufl-name="pluckedSnapPizzicatoBelow"/>-->
            <!--<glyph glyph-code="E631" smufl-name="pluckedSnapPizzicatoAbove"/>-->
            <!--<glyph glyph-code="E632" smufl-name="pluckedBuzzPizzicato"/>-->
            <!--<glyph glyph-code="E633" smufl-name="pluckedLeftHandPizzicato"/>-->
            <!--<glyph glyph-code="E634" smufl-name="arpeggiatoUp"/>-->
            <!--<glyph glyph-code="E635" smufl-name="arpeggiatoDown"/>-->
            <!--<glyph glyph-code="E636" smufl-name="pluckedWithFingernails"/>-->
            <!--<glyph glyph-code="E637" smufl-name="pluckedFingernailFlick"/>-->
            <!--<glyph glyph-code="E638" smufl-name="pluckedDamp"/>-->
            <!--<glyph glyph-code="E639" smufl-name="pluckedDampAll"/>-->
            <!--<glyph glyph-code="E63A" smufl-name="pluckedPlectrum"/>-->
            <!--<glyph glyph-code="E63B" smufl-name="pluckedDampOnStem"/>-->
            <range_end>U+E63F</range_end>
            <range_start>U+E630</range_start>
        </pluckedTechniques>
        <vocalTechniques>
            <description>Vocal techniques</description>
            <!--<glyph glyph-code="E640" smufl-name="vocalMouthClosed"/>-->
            <!--<glyph glyph-code="E641" smufl-name="vocalMouthSlightlyOpen"/>-->
            <!--<glyph glyph-code="E642" smufl-name="vocalMouthOpen"/>-->
            <!--<glyph glyph-code="E643" smufl-name="vocalMouthWideOpen"/>-->
            <!--<glyph glyph-code="E644" smufl-name="vocalMouthPursed"/>-->
            <!--<glyph glyph-code="E645" smufl-name="vocalSprechgesang"/>-->
            <!--<glyph glyph-code="E646" smufl-name="vocalsSussurando"/>-->
            <range_end>U+E64F</range_end>
            <range_start>U+E640</range_start>
        </vocalTechniques>
        <keyboardTechniques>
            <description>Keyboard techniques</description>
            <glyph glyph-code="E650" smufl-name="keyboardPedalPed"/>
            <!--<glyph glyph-code="E651" smufl-name="keyboardPedalP"/>-->
            <!--<glyph glyph-code="E652" smufl-name="keyboardPedalE"/>-->
            <!--<glyph glyph-code="E653" smufl-name="keyboardPedalD"/>-->
            <!--<glyph glyph-code="E654" smufl-name="keyboardPedalDot"/>-->
            <glyph glyph-code="E655" smufl-name="keyboardPedalUp"/>
            <!--<glyph glyph-code="E656" smufl-name="keyboardPedalHalf"/>-->
            <!--<glyph glyph-code="E657" smufl-name="keyboardPedalUpNotch"/>-->
            <!--<glyph glyph-code="E658" smufl-name="keyboardPedalHyphen"/>-->
            <glyph glyph-code="E659" smufl-name="keyboardPedalSost"/>
            <!--<glyph glyph-code="E65A" smufl-name="keyboardPedalS"/>-->
            <!--<glyph glyph-code="E65B" smufl-name="keyboardPedalHalf2"/>-->
            <!--<glyph glyph-code="E65C" smufl-name="keyboardPedalHalf3"/>-->
            <!--<glyph glyph-code="E65D" smufl-name="keyboardPedalUpSpecial"/>-->
            <!--<glyph glyph-code="E65E" smufl-name="keyboardLeftPedalPictogram"/>-->
            <!--<glyph glyph-code="E65F" smufl-name="keyboardMiddlePedalPictogram"/>-->
            <!--<glyph glyph-code="E660" smufl-name="keyboardRightPedalPictogram"/>-->
            <!--<glyph glyph-code="E661" smufl-name="keyboardPedalHeel1"/>-->
            <!--<glyph glyph-code="E662" smufl-name="keyboardPedalHeel2"/>-->
            <!--<glyph glyph-code="E663" smufl-name="keyboardPedalHeel3"/>-->
            <!--<glyph glyph-code="E664" smufl-name="keyboardPedalToe1"/>-->
            <!--<glyph glyph-code="E665" smufl-name="keyboardPedalToe2"/>-->
            <!--<glyph glyph-code="E666" smufl-name="keyboardPedalHeelToe"/>-->
            <!--<glyph glyph-code="E667" smufl-name="keyboardPluckInside"/>-->
            <!--<glyph glyph-code="E668" smufl-name="keyboardBebung2DotsAbove"/>-->
            <!--<glyph glyph-code="E669" smufl-name="keyboardBebung2DotsBelow"/>-->
            <!--<glyph glyph-code="E66A" smufl-name="keyboardBebung3DotsAbove"/>-->
            <!--<glyph glyph-code="E66B" smufl-name="keyboardBebung3DotsBelow"/>-->
            <!--<glyph glyph-code="E66C" smufl-name="keyboardBebung4DotsAbove"/>-->
            <!--<glyph glyph-code="E66D" smufl-name="keyboardBebung4DotsBelow"/>-->
            <!--<glyph glyph-code="E66E" smufl-name="keyboardPlayWithRH"/>-->
            <!--<glyph glyph-code="E66F" smufl-name="keyboardPlayWithRHEnd"/>-->
            <!--<glyph glyph-code="E670" smufl-name="keyboardPlayWithLH"/>-->
            <!--<glyph glyph-code="E671" smufl-name="keyboardPlayWithLHEnd"/>-->
            <range_end>U+E67F</range_end>
            <range_start>U+E650</range_start>
        </keyboardTechniques>
        <harpTechniques>
            <description>Harp techniques</description>
            <!--<glyph glyph-code="E680" smufl-name="harpPedalRaised"/>-->
            <!--<glyph glyph-code="E681" smufl-name="harpPedalCentered"/>-->
            <!--<glyph glyph-code="E682" smufl-name="harpPedalLowered"/>-->
            <!--<glyph glyph-code="E683" smufl-name="harpPedalDivider"/>-->
            <!--<glyph glyph-code="E684" smufl-name="harpSalzedoSlideWithSuppleness"/>-->
            <!--<glyph glyph-code="E685" smufl-name="harpSalzedoOboicFlux"/>-->
            <!--<glyph glyph-code="E686" smufl-name="harpSalzedoThunderEffect"/>-->
            <!--<glyph glyph-code="E687" smufl-name="harpSalzedoWhistlingSounds"/>-->
            <!--<glyph glyph-code="E688" smufl-name="harpSalzedoMetallicSounds"/>-->
            <!--<glyph glyph-code="E689" smufl-name="harpSalzedoTamTamSounds"/>-->
            <!--<glyph glyph-code="E68A" smufl-name="harpSalzedoPlayUpperEnd"/>-->
            <!--<glyph glyph-code="E68B" smufl-name="harpSalzedoTimpanicSounds"/>-->
            <!--<glyph glyph-code="E68C" smufl-name="harpSalzedoMuffleTotally"/>-->
            <!--<glyph glyph-code="E68D" smufl-name="harpSalzedoFluidicSoundsLeft"/>-->
            <!--<glyph glyph-code="E68E" smufl-name="harpSalzedoFluidicSoundsRight"/>-->
            <!--<glyph glyph-code="E68F" smufl-name="harpMetalRod"/>-->
            <!--<glyph glyph-code="E690" smufl-name="harpTuningKey"/>-->
            <!--<glyph glyph-code="E691" smufl-name="harpTuningKeyHandle"/>-->
            <!--<glyph glyph-code="E692" smufl-name="harpTuningKeyShank"/>-->
            <!--<glyph glyph-code="E693" smufl-name="harpTuningKeyGlissando"/>-->
            <!--<glyph glyph-code="E694" smufl-name="harpStringNoiseStem"/>-->
            <range_end>U+E69F</range_end>
            <range_start>U+E680</range_start>
        </harpTechniques>
        <tunedMalletPercussionPictograms>
            <description>Tuned mallet percussion pictograms</description>
            <!--<glyph glyph-code="E6A0" smufl-name="pictGlsp"/>-->
            <!--<glyph glyph-code="E6A1" smufl-name="pictXyl"/>-->
            <!--<glyph glyph-code="E6A2" smufl-name="pictXylTenor"/>-->
            <!--<glyph glyph-code="E6A3" smufl-name="pictXylBass"/>-->
            <!--<glyph glyph-code="E6A4" smufl-name="pictXylTrough"/>-->
            <!--<glyph glyph-code="E6A5" smufl-name="pictXylTenorTrough"/>-->
            <!--<glyph glyph-code="E6A6" smufl-name="pictMar"/>-->
            <!--<glyph glyph-code="E6A7" smufl-name="pictVib"/>-->
            <!--<glyph glyph-code="E6A8" smufl-name="pictVibMotorOff"/>-->
            <!--<glyph glyph-code="E6A9" smufl-name="pictEmptyTrap"/>-->
            <!--<glyph glyph-code="E6AA" smufl-name="pictGlspSmithBrindle"/>-->
            <!--<glyph glyph-code="E6AB" smufl-name="pictXylSmithBrindle"/>-->
            <!--<glyph glyph-code="E6AC" smufl-name="pictMarSmithBrindle"/>-->
            <!--<glyph glyph-code="E6AD" smufl-name="pictVibSmithBrindle"/>-->
            <!--<glyph glyph-code="E6AE" smufl-name="pictCrotales"/>-->
            <!--<glyph glyph-code="E6AF" smufl-name="pictSteelDrums"/>-->
            <!--<glyph glyph-code="E6B0" smufl-name="pictCelesta"/>-->
            <!--<glyph glyph-code="E6B1" smufl-name="pictLithophone"/>-->
            <!--<glyph glyph-code="E6B2" smufl-name="pictTubaphone"/>-->
            <range_end>U+E6BF</range_end>
            <range_start>U+E6A0</range_start>
        </tunedMalletPercussionPictograms>
        <chimesPictograms>
            <description>Chimes pictograms</description>
            <!--<glyph glyph-code="E6C0" smufl-name="pictTubularBells"/>-->
            <!--<glyph glyph-code="E6C1" smufl-name="pictWindChimesGlass"/>-->
            <!--<glyph glyph-code="E6C2" smufl-name="pictChimes"/>-->
            <!--<glyph glyph-code="E6C3" smufl-name="pictBambooChimes"/>-->
            <!--<glyph glyph-code="E6C4" smufl-name="pictShellChimes"/>-->
            <!--<glyph glyph-code="E6C5" smufl-name="pictGlassTubeChimes"/>-->
            <!--<glyph glyph-code="E6C6" smufl-name="pictGlassPlateChimes"/>-->
            <!--<glyph glyph-code="E6C7" smufl-name="pictMetalTubeChimes"/>-->
            <!--<glyph glyph-code="E6C8" smufl-name="pictMetalPlateChimes"/>-->
            <range_end>U+E6CF</range_end>
            <range_start>U+E6C0</range_start>
        </chimesPictograms>
        <drumsPictograms>
            <description>Drums pictograms</description>
            <!--<glyph glyph-code="E6D0" smufl-name="pictTimpani"/>-->
            <!--<glyph glyph-code="E6D1" smufl-name="pictSnareDrum"/>-->
            <!--<glyph glyph-code="E6D2" smufl-name="pictSnareDrumSnaresOff"/>-->
            <!--<glyph glyph-code="E6D3" smufl-name="pictSnareDrumMilitary"/>-->
            <!--<glyph glyph-code="E6D4" smufl-name="pictBassDrum"/>-->
            <!--<glyph glyph-code="E6D5" smufl-name="pictBassDrumOnSide"/>-->
            <!--<glyph glyph-code="E6D6" smufl-name="pictTenorDrum"/>-->
            <!--<glyph glyph-code="E6D7" smufl-name="pictTomTom"/>-->
            <!--<glyph glyph-code="E6D8" smufl-name="pictTomTomChinese"/>-->
            <!--<glyph glyph-code="E6D9" smufl-name="pictTomTomJapanese"/>-->
            <!--<glyph glyph-code="E6DA" smufl-name="pictTomTomIndoAmerican"/>-->
            <!--<glyph glyph-code="E6DB" smufl-name="pictTambourine"/>-->
            <!--<glyph glyph-code="E6DC" smufl-name="pictTimbales"/>-->
            <!--<glyph glyph-code="E6DD" smufl-name="pictBongos"/>-->
            <!--<glyph glyph-code="E6DE" smufl-name="pictConga"/>-->
            <!--<glyph glyph-code="E6DF" smufl-name="pictLogDrum"/>-->
            <!--<glyph glyph-code="E6E0" smufl-name="pictSlitDrum"/>-->
            <!--<glyph glyph-code="E6E1" smufl-name="pictBrakeDrum"/>-->
            <!--<glyph glyph-code="E6E2" smufl-name="pictGobletDrum"/>-->
            <!--<glyph glyph-code="E6E3" smufl-name="pictTabla"/>-->
            <!--<glyph glyph-code="E6E4" smufl-name="pictCuica"/>-->
            <range_end>U+E6EF</range_end>
            <range_start>U+E6D0</range_start>
        </drumsPictograms>
        <woodenStruckOrScrapedPercussionPictograms>
            <description>Wooden struck or scraped percussion pictograms</description>
            <!--<glyph glyph-code="E6F0" smufl-name="pictWoodBlock"/>-->
            <!--<glyph glyph-code="E6F1" smufl-name="pictTempleBlocks"/>-->
            <!--<glyph glyph-code="E6F2" smufl-name="pictClaves"/>-->
            <!--<glyph glyph-code="E6F3" smufl-name="pictGuiro"/>-->
            <!--<glyph glyph-code="E6F4" smufl-name="pictRatchet"/>-->
            <!--<glyph glyph-code="E6F5" smufl-name="pictFootballRatchet"/>-->
            <!--<glyph glyph-code="E6F6" smufl-name="pictWhip"/>-->
            <!--<glyph glyph-code="E6F7" smufl-name="pictBoardClapper"/>-->
            <!--<glyph glyph-code="E6F8" smufl-name="pictCastanets"/>-->
            <!--<glyph glyph-code="E6F9" smufl-name="pictCastanetsWithHandle"/>-->
            <!--<glyph glyph-code="E6FA" smufl-name="pictQuijada"/>-->
            <!--<glyph glyph-code="E6FB" smufl-name="pictBambooScraper"/>-->
            <!--<glyph glyph-code="E6FC" smufl-name="pictRecoReco"/>-->
            <range_end>U+E6FF</range_end>
            <range_start>U+E6F0</range_start>
        </woodenStruckOrScrapedPercussionPictograms>
        <metallicStruckPercussionPictograms>
            <description>Metallic struck percussion pictograms</description>
            <!--<glyph glyph-code="E700" smufl-name="pictTriangle"/>-->
            <!--<glyph glyph-code="E701" smufl-name="pictAnvil"/>-->
            <range_end>U+E70F</range_end>
            <range_start>U+E700</range_start>
        </metallicStruckPercussionPictograms>
        <bellsPictograms>
            <description>Bells pictograms</description>
            <!--<glyph glyph-code="E710" smufl-name="pictSleighBell"/>-->
            <!--<glyph glyph-code="E711" smufl-name="pictCowBell"/>-->
            <!--<glyph glyph-code="E712" smufl-name="pictAlmglocken"/>-->
            <!--<glyph glyph-code="E713" smufl-name="pictBellPlate"/>-->
            <!--<glyph glyph-code="E714" smufl-name="pictBell"/>-->
            <!--<glyph glyph-code="E715" smufl-name="pictHandbell"/>-->
            <!--<glyph glyph-code="E716" smufl-name="pictCencerro"/>-->
            <!--<glyph glyph-code="E717" smufl-name="pictAgogo"/>-->
            <!--<glyph glyph-code="E718" smufl-name="pictShellBells"/>-->
            <!--<glyph glyph-code="E719" smufl-name="pictJingleBells"/>-->
            <!--<glyph glyph-code="E71A" smufl-name="pictBellTree"/>-->
            <range_end>U+E71F</range_end>
            <range_start>U+E710</range_start>
        </bellsPictograms>
        <cymbalsPictograms>
            <description>Cymbals pictograms</description>
            <!--<glyph glyph-code="E720" smufl-name="pictCrashCymbals"/>-->
            <!--<glyph glyph-code="E721" smufl-name="pictSuspendedCymbal"/>-->
            <!--<glyph glyph-code="E722" smufl-name="pictHiHat"/>-->
            <!--<glyph glyph-code="E723" smufl-name="pictHiHatOnStand"/>-->
            <!--<glyph glyph-code="E724" smufl-name="pictSizzleCymbal"/>-->
            <!--<glyph glyph-code="E725" smufl-name="pictVietnameseHat"/>-->
            <!--<glyph glyph-code="E726" smufl-name="pictChineseCymbal"/>-->
            <!--<glyph glyph-code="E727" smufl-name="pictFingerCymbals"/>-->
            <!--<glyph glyph-code="E728" smufl-name="pictCymbalTongs"/>-->
            <!--<glyph glyph-code="E729" smufl-name="pictEdgeOfCymbal"/>-->
            <!--<glyph glyph-code="E72A" smufl-name="pictBellOfCymbal"/>-->
            <range_end>U+E72F</range_end>
            <range_start>U+E720</range_start>
        </cymbalsPictograms>
        <gongsPictograms>
            <description>Gongs pictograms</description>
            <!--<glyph glyph-code="E730" smufl-name="pictTamTam"/>-->
            <!--<glyph glyph-code="E731" smufl-name="pictTamTamWithBeater"/>-->
            <!--<glyph glyph-code="E732" smufl-name="pictGong"/>-->
            <!--<glyph glyph-code="E733" smufl-name="pictGongWithButton"/>-->
            <!--<glyph glyph-code="E734" smufl-name="pictSlideBrushOnGong"/>-->
            <range_end>U+E73F</range_end>
            <range_start>U+E730</range_start>
        </gongsPictograms>
        <shakersOrRattlesPictograms>
            <description>Shakers or rattles pictograms</description>
            <!--<glyph glyph-code="E740" smufl-name="pictFlexatone"/>-->
            <!--<glyph glyph-code="E741" smufl-name="pictMaraca"/>-->
            <!--<glyph glyph-code="E742" smufl-name="pictMaracas"/>-->
            <!--<glyph glyph-code="E743" smufl-name="pictCabasa"/>-->
            <!--<glyph glyph-code="E744" smufl-name="pictThundersheet"/>-->
            <!--<glyph glyph-code="E745" smufl-name="pictVibraslap"/>-->
            <!--<glyph glyph-code="E746" smufl-name="pictSistrum"/>-->
            <!--<glyph glyph-code="E747" smufl-name="pictRainstick"/>-->
            <!--<glyph glyph-code="E748" smufl-name="pictChainRattle"/>-->
            <range_end>U+E74F</range_end>
            <range_start>U+E740</range_start>
        </shakersOrRattlesPictograms>
        <whistlesAndAerophonesPictograms>
            <description>Whistles and aerophones pictograms</description>
            <!--<glyph glyph-code="E750" smufl-name="pictSlideWhistle"/>-->
            <!--<glyph glyph-code="E751" smufl-name="pictBirdWhistle"/>-->
            <!--<glyph glyph-code="E752" smufl-name="pictPoliceWhistle"/>-->
            <!--<glyph glyph-code="E753" smufl-name="pictSiren"/>-->
            <!--<glyph glyph-code="E754" smufl-name="pictWindMachine"/>-->
            <!--<glyph glyph-code="E755" smufl-name="pictCarHorn"/>-->
            <!--<glyph glyph-code="E756" smufl-name="pictKlaxonHorn"/>-->
            <!--<glyph glyph-code="E757" smufl-name="pictDuckCall"/>-->
            <!--<glyph glyph-code="E758" smufl-name="pictWindWhistle"/>-->
            <!--<glyph glyph-code="E759" smufl-name="pictMegaphone"/>-->
            <!--<glyph glyph-code="E75A" smufl-name="pictLotusFlute"/>-->
            <range_end>U+E75F</range_end>
            <range_start>U+E750</range_start>
        </whistlesAndAerophonesPictograms>
        <miscellaneousPercussionInstrumentPictograms>
            <description>Miscellaneous percussion instrument pictograms</description>
            <!--<glyph glyph-code="E760" smufl-name="pictPistolShot"/>-->
            <!--<glyph glyph-code="E761" smufl-name="pictCannon"/>-->
            <!--<glyph glyph-code="E762" smufl-name="pictSandpaperBlocks"/>-->
            <!--<glyph glyph-code="E763" smufl-name="pictLionsRoar"/>-->
            <!--<glyph glyph-code="E764" smufl-name="pictGlassHarp"/>-->
            <!--<glyph glyph-code="E765" smufl-name="pictGlassHarmonica"/>-->
            <!--<glyph glyph-code="E766" smufl-name="pictMusicalSaw"/>-->
            <!--<glyph glyph-code="E767" smufl-name="pictJawHarp"/>-->
            <range_end>U+E76F</range_end>
            <range_start>U+E760</range_start>
        </miscellaneousPercussionInstrumentPictograms>
        <beatersPictograms>
            <description>Beaters pictograms</description>
            <!--<glyph glyph-code="E770" smufl-name="pictBeaterSoftXylophoneUp"/>-->
            <!--<glyph glyph-code="E771" smufl-name="pictBeaterSoftXylophoneDown"/>-->
            <!--<glyph glyph-code="E772" smufl-name="pictBeaterSoftXylophoneRight"/>-->
            <!--<glyph glyph-code="E773" smufl-name="pictBeaterSoftXylophoneLeft"/>-->
            <!--<glyph glyph-code="E774" smufl-name="pictBeaterMediumXylophoneUp"/>-->
            <!--<glyph glyph-code="E775" smufl-name="pictBeaterMediumXylophoneDown"/>-->
            <!--<glyph glyph-code="E776" smufl-name="pictBeaterMediumXylophoneRight"/>-->
            <!--<glyph glyph-code="E777" smufl-name="pictBeaterMediumXylophoneLeft"/>-->
            <!--<glyph glyph-code="E778" smufl-name="pictBeaterHardXylophoneUp"/>-->
            <!--<glyph glyph-code="E779" smufl-name="pictBeaterHardXylophoneDown"/>-->
            <!--<glyph glyph-code="E77A" smufl-name="pictBeaterHardXylophoneRight"/>-->
            <!--<glyph glyph-code="E77B" smufl-name="pictBeaterHardXylophoneLeft"/>-->
            <!--<glyph glyph-code="E77C" smufl-name="pictBeaterWoodXylophoneUp"/>-->
            <!--<glyph glyph-code="E77D" smufl-name="pictBeaterWoodXylophoneDown"/>-->
            <!--<glyph glyph-code="E77E" smufl-name="pictBeaterWoodXylophoneRight"/>-->
            <!--<glyph glyph-code="E77F" smufl-name="pictBeaterWoodXylophoneLeft"/>-->
            <!--<glyph glyph-code="E780" smufl-name="pictBeaterSoftGlockenspielUp"/>-->
            <!--<glyph glyph-code="E781" smufl-name="pictBeaterSoftGlockenspielDown"/>-->
            <!--<glyph glyph-code="E782" smufl-name="pictBeaterSoftGlockenspielRight"/>-->
            <!--<glyph glyph-code="E783" smufl-name="pictBeaterSoftGlockenspielLeft"/>-->
            <!--<glyph glyph-code="E784" smufl-name="pictBeaterHardGlockenspielUp"/>-->
            <!--<glyph glyph-code="E785" smufl-name="pictBeaterHardGlockenspielDown"/>-->
            <!--<glyph glyph-code="E786" smufl-name="pictBeaterHardGlockenspielRight"/>-->
            <!--<glyph glyph-code="E787" smufl-name="pictBeaterHardGlockenspielLeft"/>-->
            <!--<glyph glyph-code="E788" smufl-name="pictBeaterSoftTimpaniUp"/>-->
            <!--<glyph glyph-code="E789" smufl-name="pictBeaterSoftTimpaniDown"/>-->
            <!--<glyph glyph-code="E78A" smufl-name="pictBeaterSoftTimpaniRight"/>-->
            <!--<glyph glyph-code="E78B" smufl-name="pictBeaterSoftTimpaniLeft"/>-->
            <!--<glyph glyph-code="E78C" smufl-name="pictBeaterMediumTimpaniUp"/>-->
            <!--<glyph glyph-code="E78D" smufl-name="pictBeaterMediumTimpaniDown"/>-->
            <!--<glyph glyph-code="E78E" smufl-name="pictBeaterMediumTimpaniRight"/>-->
            <!--<glyph glyph-code="E78F" smufl-name="pictBeaterMediumTimpaniLeft"/>-->
            <!--<glyph glyph-code="E790" smufl-name="pictBeaterHardTimpaniUp"/>-->
            <!--<glyph glyph-code="E791" smufl-name="pictBeaterHardTimpaniDown"/>-->
            <!--<glyph glyph-code="E792" smufl-name="pictBeaterHardTimpaniRight"/>-->
            <!--<glyph glyph-code="E793" smufl-name="pictBeaterHardTimpaniLeft"/>-->
            <!--<glyph glyph-code="E794" smufl-name="pictBeaterWoodTimpaniUp"/>-->
            <!--<glyph glyph-code="E795" smufl-name="pictBeaterWoodTimpaniDown"/>-->
            <!--<glyph glyph-code="E796" smufl-name="pictBeaterWoodTimpaniRight"/>-->
            <!--<glyph glyph-code="E797" smufl-name="pictBeaterWoodTimpaniLeft"/>-->
            <!--<glyph glyph-code="E798" smufl-name="pictBeaterSoftBassDrumUp"/>-->
            <!--<glyph glyph-code="E799" smufl-name="pictBeaterSoftBassDrumDown"/>-->
            <!--<glyph glyph-code="E79A" smufl-name="pictBeaterMediumBassDrumUp"/>-->
            <!--<glyph glyph-code="E79B" smufl-name="pictBeaterMediumBassDrumDown"/>-->
            <!--<glyph glyph-code="E79C" smufl-name="pictBeaterHardBassDrumUp"/>-->
            <!--<glyph glyph-code="E79D" smufl-name="pictBeaterHardBassDrumDown"/>-->
            <!--<glyph glyph-code="E79E" smufl-name="pictBeaterMetalBassDrumUp"/>-->
            <!--<glyph glyph-code="E79F" smufl-name="pictBeaterMetalBassDrumDown"/>-->
            <!--<glyph glyph-code="E7A0" smufl-name="pictBeaterDoubleBassDrumUp"/>-->
            <!--<glyph glyph-code="E7A1" smufl-name="pictBeaterDoubleBassDrumDown"/>-->
            <!--<glyph glyph-code="E7A2" smufl-name="pictBeaterSoftYarnUp"/>-->
            <!--<glyph glyph-code="E7A3" smufl-name="pictBeaterSoftYarnDown"/>-->
            <!--<glyph glyph-code="E7A4" smufl-name="pictBeaterSoftYarnRight"/>-->
            <!--<glyph glyph-code="E7A5" smufl-name="pictBeaterSoftYarnLeft"/>-->
            <!--<glyph glyph-code="E7A6" smufl-name="pictBeaterMediumYarnUp"/>-->
            <!--<glyph glyph-code="E7A7" smufl-name="pictBeaterMediumYarnDown"/>-->
            <!--<glyph glyph-code="E7A8" smufl-name="pictBeaterMediumYarnRight"/>-->
            <!--<glyph glyph-code="E7A9" smufl-name="pictBeaterMediumYarnLeft"/>-->
            <!--<glyph glyph-code="E7AA" smufl-name="pictBeaterHardYarnUp"/>-->
            <!--<glyph glyph-code="E7AB" smufl-name="pictBeaterHardYarnDown"/>-->
            <!--<glyph glyph-code="E7AC" smufl-name="pictBeaterHardYarnRight"/>-->
            <!--<glyph glyph-code="E7AD" smufl-name="pictBeaterHardYarnLeft"/>-->
            <!--<glyph glyph-code="E7AE" smufl-name="pictBeaterSuperballUp"/>-->
            <!--<glyph glyph-code="E7AF" smufl-name="pictBeaterSuperballDown"/>-->
            <!--<glyph glyph-code="E7B0" smufl-name="pictBeaterSuperballRight"/>-->
            <!--<glyph glyph-code="E7B1" smufl-name="pictBeaterSuperballLeft"/>-->
            <!--<glyph glyph-code="E7B2" smufl-name="pictSuperball"/>-->
            <!--<glyph glyph-code="E7B3" smufl-name="pictWoundHardUp"/>-->
            <!--<glyph glyph-code="E7B4" smufl-name="pictWoundHardDown"/>-->
            <!--<glyph glyph-code="E7B5" smufl-name="pictWoundHardRight"/>-->
            <!--<glyph glyph-code="E7B6" smufl-name="pictWoundHardLeft"/>-->
            <!--<glyph glyph-code="E7B7" smufl-name="pictWoundSoftUp"/>-->
            <!--<glyph glyph-code="E7B8" smufl-name="pictWoundSoftDown"/>-->
            <!--<glyph glyph-code="E7B9" smufl-name="pictWoundSoftRight"/>-->
            <!--<glyph glyph-code="E7BA" smufl-name="pictWoundSoftLeft"/>-->
            <!--<glyph glyph-code="E7BB" smufl-name="pictGumSoftUp"/>-->
            <!--<glyph glyph-code="E7BC" smufl-name="pictGumSoftDown"/>-->
            <!--<glyph glyph-code="E7BD" smufl-name="pictGumSoftRight"/>-->
            <!--<glyph glyph-code="E7BE" smufl-name="pictGumSoftLeft"/>-->
            <!--<glyph glyph-code="E7BF" smufl-name="pictGumMediumUp"/>-->
            <!--<glyph glyph-code="E7C0" smufl-name="pictGumMediumDown"/>-->
            <!--<glyph glyph-code="E7C1" smufl-name="pictGumMediumRight"/>-->
            <!--<glyph glyph-code="E7C2" smufl-name="pictGumMediumLeft"/>-->
            <!--<glyph glyph-code="E7C3" smufl-name="pictGumHardUp"/>-->
            <!--<glyph glyph-code="E7C4" smufl-name="pictGumHardDown"/>-->
            <!--<glyph glyph-code="E7C5" smufl-name="pictGumHardRight"/>-->
            <!--<glyph glyph-code="E7C6" smufl-name="pictGumHardLeft"/>-->
            <!--<glyph glyph-code="E7C7" smufl-name="pictBeaterMetalUp"/>-->
            <!--<glyph glyph-code="E7C8" smufl-name="pictBeaterMetalDown"/>-->
            <!--<glyph glyph-code="E7C9" smufl-name="pictBeaterMetalRight"/>-->
            <!--<glyph glyph-code="E7CA" smufl-name="pictBeaterMetalLeft"/>-->
            <!--<glyph glyph-code="E7CB" smufl-name="pictBeaterHammerWoodUp"/>-->
            <!--<glyph glyph-code="E7CC" smufl-name="pictBeaterHammerWoodDown"/>-->
            <!--<glyph glyph-code="E7CD" smufl-name="pictBeaterHammerPlasticUp"/>-->
            <!--<glyph glyph-code="E7CE" smufl-name="pictBeaterHammerPlasticDown"/>-->
            <!--<glyph glyph-code="E7CF" smufl-name="pictBeaterHammerMetalUp"/>-->
            <!--<glyph glyph-code="E7D0" smufl-name="pictBeaterHammerMetalDown"/>-->
            <!--<glyph glyph-code="E7D1" smufl-name="pictBeaterSnareSticksUp"/>-->
            <!--<glyph glyph-code="E7D2" smufl-name="pictBeaterSnareSticksDown"/>-->
            <!--<glyph glyph-code="E7D3" smufl-name="pictBeaterJazzSticksUp"/>-->
            <!--<glyph glyph-code="E7D4" smufl-name="pictBeaterJazzSticksDown"/>-->
            <!--<glyph glyph-code="E7D5" smufl-name="pictBeaterTriangleUp"/>-->
            <!--<glyph glyph-code="E7D6" smufl-name="pictBeaterTriangleDown"/>-->
            <!--<glyph glyph-code="E7D7" smufl-name="pictBeaterWireBrushesUp"/>-->
            <!--<glyph glyph-code="E7D8" smufl-name="pictBeaterWireBrushesDown"/>-->
            <!--<glyph glyph-code="E7D9" smufl-name="pictBeaterBrassMalletsUp"/>-->
            <!--<glyph glyph-code="E7DA" smufl-name="pictBeaterBrassMalletsDown"/>-->
            <!--<glyph glyph-code="E7DB" smufl-name="pictBeaterSoftXylophone"/>-->
            <!--<glyph glyph-code="E7DC" smufl-name="pictBeaterSpoonWoodenMallet"/>-->
            <!--<glyph glyph-code="E7DD" smufl-name="pictBeaterGuiroScraper"/>-->
            <!--<glyph glyph-code="E7DE" smufl-name="pictBeaterBow"/>-->
            <!--<glyph glyph-code="E7DF" smufl-name="pictBeaterMallet"/>-->
            <!--<glyph glyph-code="E7E0" smufl-name="pictBeaterMetalHammer"/>-->
            <!--<glyph glyph-code="E7E1" smufl-name="pictBeaterHammer"/>-->
            <!--<glyph glyph-code="E7E2" smufl-name="pictBeaterKnittingNeedle"/>-->
            <!--<glyph glyph-code="E7E3" smufl-name="pictBeaterHand"/>-->
            <!--<glyph glyph-code="E7E4" smufl-name="pictBeaterFinger"/>-->
            <!--<glyph glyph-code="E7E5" smufl-name="pictBeaterFist"/>-->
            <!--<glyph glyph-code="E7E6" smufl-name="pictBeaterFingernails"/>-->
            <!--<glyph glyph-code="E7E7" smufl-name="pictCoins"/>-->
            <!--<glyph glyph-code="E7E8" smufl-name="pictDrumStick"/>-->
            <!--<glyph glyph-code="E7E9" smufl-name="pictBeaterCombiningParentheses"/>-->
            <!--<glyph glyph-code="E7EA" smufl-name="pictBeaterCombiningDashedCircle"/>-->
            <!--<glyph glyph-code="E7EB" smufl-name="pictBeaterBox"/>-->
            <range_end>U+E7EF</range_end>
            <range_start>U+E770</range_start>
        </beatersPictograms>
        <percussionPlayingTechniquePictograms>
            <description>Percussion playing technique pictograms</description>
            <!--<glyph glyph-code="E7F0" smufl-name="pictStickShot"/>-->
            <!--<glyph glyph-code="E7F1" smufl-name="pictScrapeCenterToEdge"/>-->
            <!--<glyph glyph-code="E7F2" smufl-name="pictScrapeEdgeToCenter"/>-->
            <!--<glyph glyph-code="E7F3" smufl-name="pictScrapeAroundRim"/>-->
            <!--<glyph glyph-code="E7F4" smufl-name="pictOnRim"/>-->
            <!--<glyph glyph-code="E7F5" smufl-name="pictOpenRimShot"/>-->
            <!--<glyph glyph-code="E7F6" smufl-name="pictHalfOpen1"/>-->
            <!--<glyph glyph-code="E7F7" smufl-name="pictHalfOpen2"/>-->
            <!--<glyph glyph-code="E7F8" smufl-name="pictOpen"/>-->
            <!--<glyph glyph-code="E7F9" smufl-name="pictDamp1"/>-->
            <!--<glyph glyph-code="E7FA" smufl-name="pictDamp2"/>-->
            <!--<glyph glyph-code="E7FB" smufl-name="pictDamp3"/>-->
            <!--<glyph glyph-code="E7FC" smufl-name="pictDamp4"/>-->
            <!--<glyph glyph-code="E7FD" smufl-name="pictRimShotOnStem"/>-->
            <!--<glyph glyph-code="E7FE" smufl-name="pictCenter1"/>-->
            <!--<glyph glyph-code="E7FF" smufl-name="pictCenter2"/>-->
            <!--<glyph glyph-code="E800" smufl-name="pictCenter3"/>-->
            <!--<glyph glyph-code="E801" smufl-name="pictRim1"/>-->
            <!--<glyph glyph-code="E802" smufl-name="pictRim2"/>-->
            <!--<glyph glyph-code="E803" smufl-name="pictRim3"/>-->
            <!--<glyph glyph-code="E804" smufl-name="pictNormalPosition"/>-->
            <!--<glyph glyph-code="E805" smufl-name="pictChokeCymbal"/>-->
            <!--<glyph glyph-code="E806" smufl-name="pictRightHandSquare"/>-->
            <!--<glyph glyph-code="E807" smufl-name="pictLeftHandCircle"/>-->
            <!--<glyph glyph-code="E808" smufl-name="pictSwishStem"/>-->
            <!--<glyph glyph-code="E809" smufl-name="pictTurnRightStem"/>-->
            <!--<glyph glyph-code="E80A" smufl-name="pictTurnLeftStem"/>-->
            <!--<glyph glyph-code="E80B" smufl-name="pictTurnRightLeftStem"/>-->
            <!--<glyph glyph-code="E80C" smufl-name="pictCrushStem"/>-->
            <!--<glyph glyph-code="E80D" smufl-name="pictDeadNoteStem"/>-->
            <range_end>U+E80F</range_end>
            <range_start>U+E7F0</range_start>
        </percussionPlayingTechniquePictograms>
        <handbells>
            <description>Handbells</description>
            <!--<glyph glyph-code="E810" smufl-name="handbellsMartellato"/>-->
            <!--<glyph glyph-code="E811" smufl-name="handbellsMartellatoLift"/>-->
            <!--<glyph glyph-code="E812" smufl-name="handbellsHandMartellato"/>-->
            <!--<glyph glyph-code="E813" smufl-name="handbellsMutedMartellato"/>-->
            <!--<glyph glyph-code="E814" smufl-name="handbellsMalletBellSuspended"/>-->
            <!--<glyph glyph-code="E815" smufl-name="handbellsMalletBellOnTable"/>-->
            <!--<glyph glyph-code="E816" smufl-name="handbellsMalletLft"/>-->
            <!--<glyph glyph-code="E817" smufl-name="handbellsPluckLift"/>-->
            <!--<glyph glyph-code="E818" smufl-name="handbellsSwingUp"/>-->
            <!--<glyph glyph-code="E819" smufl-name="handbellsSwingDown"/>-->
            <!--<glyph glyph-code="E81A" smufl-name="handbellsSwing"/>-->
            <!--<glyph glyph-code="E81B" smufl-name="handbellsEcho1"/>-->
            <!--<glyph glyph-code="E81C" smufl-name="handbellsEcho2"/>-->
            <!--<glyph glyph-code="E81D" smufl-name="handbellsGyro"/>-->
            <!--<glyph glyph-code="E81E" smufl-name="handbellsDamp3"/>-->
            <!--<glyph glyph-code="E81F" smufl-name="handbellsBelltree"/>-->
            <!--<glyph glyph-code="E820" smufl-name="handbellsTableSingleBell"/>-->
            <!--<glyph glyph-code="E821" smufl-name="handbellsTablePairBells"/>-->
            <range_end>U+E82F</range_end>
            <range_start>U+E810</range_start>
        </handbells>
        <guitar>
            <description>Guitar</description>
            <!--<glyph glyph-code="E830" smufl-name="guitarVibratoBarScoop"/>-->
            <!--<glyph glyph-code="E831" smufl-name="guitarVibratoBarDip"/>-->
            <!--<glyph glyph-code="E832" smufl-name="guitarShake"/>-->
            <!--<glyph glyph-code="E833" smufl-name="guitarString0"/>-->
            <!--<glyph glyph-code="E834" smufl-name="guitarString1"/>-->
            <!--<glyph glyph-code="E835" smufl-name="guitarString2"/>-->
            <!--<glyph glyph-code="E836" smufl-name="guitarString3"/>-->
            <!--<glyph glyph-code="E837" smufl-name="guitarString4"/>-->
            <!--<glyph glyph-code="E838" smufl-name="guitarString5"/>-->
            <!--<glyph glyph-code="E839" smufl-name="guitarString6"/>-->
            <!--<glyph glyph-code="E83A" smufl-name="guitarString7"/>-->
            <!--<glyph glyph-code="E83B" smufl-name="guitarString8"/>-->
            <!--<glyph glyph-code="E83C" smufl-name="guitarString9"/>-->
            <!--<glyph glyph-code="E83D" smufl-name="guitarOpenPedal"/>-->
            <!--<glyph glyph-code="E83E" smufl-name="guitarHalfOpenPedal"/>-->
            <!--<glyph glyph-code="E83F" smufl-name="guitarClosePedal"/>-->
            <!--<glyph glyph-code="E840" smufl-name="guitarLeftHandTapping"/>-->
            <!--<glyph glyph-code="E841" smufl-name="guitarRightHandTapping"/>-->
            <!--<glyph glyph-code="E842" smufl-name="guitarGolpe"/>-->
            <!--<glyph glyph-code="E843" smufl-name="guitarFadeIn"/>-->
            <!--<glyph glyph-code="E844" smufl-name="guitarFadeOut"/>-->
            <!--<glyph glyph-code="E845" smufl-name="guitarVolumeSwell"/>-->
            <!--<glyph glyph-code="E846" smufl-name="guitarStrumUp"/>-->
            <!--<glyph glyph-code="E847" smufl-name="guitarStrumDown"/>-->
            <range_end>U+E84F</range_end>
            <range_start>U+E830</range_start>
        </guitar>
        <chordDiagrams>
            <description>Chord diagrams</description>
            <!--<glyph glyph-code="E850" smufl-name="fretboard3String"/>-->
            <!--<glyph glyph-code="E851" smufl-name="fretboard3StringNut"/>-->
            <!--<glyph glyph-code="E852" smufl-name="fretboard4String"/>-->
            <!--<glyph glyph-code="E853" smufl-name="fretboard4StringNut"/>-->
            <!--<glyph glyph-code="E854" smufl-name="fretboard5String"/>-->
            <!--<glyph glyph-code="E855" smufl-name="fretboard5StringNut"/>-->
            <!--<glyph glyph-code="E856" smufl-name="fretboard6String"/>-->
            <!--<glyph glyph-code="E857" smufl-name="fretboard6StringNut"/>-->
            <!--<glyph glyph-code="E858" smufl-name="fretboardFilledCircle"/>-->
            <!--<glyph glyph-code="E859" smufl-name="fretboardX"/>-->
            <!--<glyph glyph-code="E85A" smufl-name="fretboardO"/>-->
            <range_end>U+E85F</range_end>
            <range_start>U+E850</range_start>
        </chordDiagrams>
        <analytics>
            <description>Analytics</description>
            <!--<glyph glyph-code="E860" smufl-name="analyticsHauptstimme"/>-->
            <!--<glyph glyph-code="E861" smufl-name="analyticsNebenstimme"/>-->
            <!--<glyph glyph-code="E862" smufl-name="analyticsStartStimme"/>-->
            <!--<glyph glyph-code="E863" smufl-name="analyticsEndStimme"/>-->
            <!--<glyph glyph-code="E864" smufl-name="analyticsTheme"/>-->
            <!--<glyph glyph-code="E865" smufl-name="analyticsThemeRetrograde"/>-->
            <!--<glyph glyph-code="E866" smufl-name="analyticsThemeRetrogradeInversion"/>-->
            <!--<glyph glyph-code="E867" smufl-name="analyticsThemeInversion"/>-->
            <!--<glyph glyph-code="E868" smufl-name="analyticsTheme1"/>-->
            <!--<glyph glyph-code="E869" smufl-name="analyticsInversion1"/>-->
            <range_end>U+E86F</range_end>
            <range_start>U+E860</range_start>
        </analytics>
        <chordSymbols>
            <description>Chord symbols</description>
            <!--<glyph glyph-code="E870" smufl-name="csymDiminished"/>-->
            <!--<glyph glyph-code="E871" smufl-name="csymHalfDiminished"/>-->
            <!--<glyph glyph-code="E872" smufl-name="csymAugmented"/>-->
            <!--<glyph glyph-code="E873" smufl-name="csymMajorSeventh"/>-->
            <!--<glyph glyph-code="E874" smufl-name="csymMinor"/>-->
            <!--<glyph glyph-code="E875" smufl-name="csymParensLeftTall"/>-->
            <!--<glyph glyph-code="E876" smufl-name="csymParensRightTall"/>-->
            <!--<glyph glyph-code="E877" smufl-name="csymBracketLeftTall"/>-->
            <!--<glyph glyph-code="E878" smufl-name="csymBracketRightTall"/>-->
            <range_end>U+E87F</range_end>
            <range_start>U+E870</range_start>
        </chordSymbols>
        <tuplets>
            <description>Tuplets</description>
            <glyph glyph-code="E880" smufl-name="tuplet0"/>
            <glyph glyph-code="E881" smufl-name="tuplet1"/>
            <glyph glyph-code="E882" smufl-name="tuplet2"/>
            <glyph glyph-code="E883" smufl-name="tuplet3"/>
            <glyph glyph-code="E884" smufl-name="tuplet4"/>
            <glyph glyph-code="E885" smufl-name="tuplet5"/>
            <glyph glyph-code="E886" smufl-name="tuplet6"/>
            <glyph glyph-code="E887" smufl-name="tuplet7"/>
            <glyph glyph-code="E888" smufl-name="tuplet8"/>
            <glyph glyph-code="E889" smufl-name="tuplet9"/>
            <glyph glyph-code="E88A" smufl-name="tupletColon"/>
            <range_end>U+E88F</range_end>
            <range_start>U+E880</range_start>
        </tuplets>
        <conductorSymbols>
            <description>Conductor symbols</description>
            <!--<glyph glyph-code="E890" smufl-name="conductorStrongBeat"/>-->
            <!--<glyph glyph-code="E891" smufl-name="conductorLeftBeat"/>-->
            <!--<glyph glyph-code="E892" smufl-name="conductorRightBeat"/>-->
            <!--<glyph glyph-code="E893" smufl-name="conductorWeakBeat"/>-->
            <!--<glyph glyph-code="E894" smufl-name="conductorBeat2Simple"/>-->
            <!--<glyph glyph-code="E895" smufl-name="conductorBeat3Simple"/>-->
            <!--<glyph glyph-code="E896" smufl-name="conductorBeat4Simple"/>-->
            <!--<glyph glyph-code="E897" smufl-name="conductorBeat2Compound"/>-->
            <!--<glyph glyph-code="E898" smufl-name="conductorBeat3Compound"/>-->
            <!--<glyph glyph-code="E899" smufl-name="conductorBeat4Compound"/>-->
            <range_end>U+E89F</range_end>
            <range_start>U+E890</range_start>
        </conductorSymbols>
        <accordion>
            <description>Accordion</description>
            <!--<glyph glyph-code="E8A0" smufl-name="accdnRH3RanksPiccolo"/>-->
            <!--<glyph glyph-code="E8A1" smufl-name="accdnRH3RanksClarinet"/>-->
            <!--<glyph glyph-code="E8A2" smufl-name="accdnRH3RanksUpperTremolo8"/>-->
            <!--<glyph glyph-code="E8A3" smufl-name="accdnRH3RanksLowerTremolo8"/>-->
            <!--<glyph glyph-code="E8A4" smufl-name="accdnRH3RanksBassoon"/>-->
            <!--<glyph glyph-code="E8A5" smufl-name="accdnRH3RanksOboe"/>-->
            <!--<glyph glyph-code="E8A6" smufl-name="accdnRH3RanksViolin"/>-->
            <!--<glyph glyph-code="E8A7" smufl-name="accdnRH3RanksImitationMusette"/>-->
            <!--<glyph glyph-code="E8A8" smufl-name="accdnRH3RanksAuthenticMusette"/>-->
            <!--<glyph glyph-code="E8A9" smufl-name="accdnRH3RanksOrgan"/>-->
            <!--<glyph glyph-code="E8AA" smufl-name="accdnRH3RanksHarmonium"/>-->
            <!--<glyph glyph-code="E8AB" smufl-name="accdnRH3RanksBandoneon"/>-->
            <!--<glyph glyph-code="E8AC" smufl-name="accdnRH3RanksAccordion"/>-->
            <!--<glyph glyph-code="E8AD" smufl-name="accdnRH3RanksMaster"/>-->
            <!--<glyph glyph-code="E8AE" smufl-name="accdnRH3RanksTwoChoirs"/>-->
            <!--<glyph glyph-code="E8AF" smufl-name="accdnRH3RanksTremoloLower8ve"/>-->
            <!--<glyph glyph-code="E8B0" smufl-name="accdnRH3RanksTremoloUpper8ve"/>-->
            <!--<glyph glyph-code="E8B1" smufl-name="accdnRH3RanksDoubleTremoloLower8ve"/>-->
            <!--<glyph glyph-code="E8B2" smufl-name="accdnRH3RanksDoubleTremoloUpper8ve"/>-->
            <!--<glyph glyph-code="E8B3" smufl-name="accdnRH3RanksFullFactory"/>-->
            <!--<glyph glyph-code="E8B4" smufl-name="accdnRH4RanksSoprano"/>-->
            <!--<glyph glyph-code="E8B5" smufl-name="accdnRH4RanksAlto"/>-->
            <!--<glyph glyph-code="E8B6" smufl-name="accdnRH4RanksTenor"/>-->
            <!--<glyph glyph-code="E8B7" smufl-name="accdnRH4RanksMaster"/>-->
            <!--<glyph glyph-code="E8B8" smufl-name="accdnRH4RanksSoftBass"/>-->
            <!--<glyph glyph-code="E8B9" smufl-name="accdnRH4RanksSoftTenor"/>-->
            <!--<glyph glyph-code="E8BA" smufl-name="accdnRH4RanksBassAlto"/>-->
            <!--<glyph glyph-code="E8BB" smufl-name="accdnLH2Ranks8Round"/>-->
            <!--<glyph glyph-code="E8BC" smufl-name="accdnLH2Ranks16Round"/>-->
            <!--<glyph glyph-code="E8BD" smufl-name="accdnLH2Ranks8Plus16Round"/>-->
            <!--<glyph glyph-code="E8BE" smufl-name="accdnLH2RanksMasterRound"/>-->
            <!--<glyph glyph-code="E8BF" smufl-name="accdnLH2RanksMasterPlus16Round"/>-->
            <!--<glyph glyph-code="E8C0" smufl-name="accdnLH2RanksFullMasterRound"/>-->
            <!--<glyph glyph-code="E8C1" smufl-name="accdnLH3Ranks8Square"/>-->
            <!--<glyph glyph-code="E8C2" smufl-name="accdnLH3Ranks2Square"/>-->
            <!--<glyph glyph-code="E8C3" smufl-name="accdnLH3RanksDouble8Square"/>-->
            <!--<glyph glyph-code="E8C4" smufl-name="accdnLH3Ranks2Plus8Square"/>-->
            <!--<glyph glyph-code="E8C5" smufl-name="accdnLH3RanksTuttiSquare"/>-->
            <!--<glyph glyph-code="E8C6" smufl-name="accdnCombRH3RanksEmpty"/>-->
            <!--<glyph glyph-code="E8C7" smufl-name="accdnCombRH4RanksEmpty"/>-->
            <!--<glyph glyph-code="E8C8" smufl-name="accdnCombLH2RanksEmpty"/>-->
            <!--<glyph glyph-code="E8C9" smufl-name="accdnCombLH3RanksEmptySquare"/>-->
            <!--<glyph glyph-code="E8CA" smufl-name="accdnCombDot"/>-->
            <!--<glyph glyph-code="E8CB" smufl-name="accdnPush"/>-->
            <!--<glyph glyph-code="E8CC" smufl-name="accdnPull"/>-->
            <!--<glyph glyph-code="E8CD" smufl-name="accdnRicochet2"/>-->
            <!--<glyph glyph-code="E8CE" smufl-name="accdnRicochet3"/>-->
            <!--<glyph glyph-code="E8CF" smufl-name="accdnRicochet4"/>-->
            <!--<glyph glyph-code="E8D0" smufl-name="accdnRicochet5"/>-->
            <!--<glyph glyph-code="E8D1" smufl-name="accdnRicochet6"/>-->
            <!--<glyph glyph-code="E8D2" smufl-name="accdnRicochetStem2"/>-->
            <!--<glyph glyph-code="E8D3" smufl-name="accdnRicochetStem3"/>-->
            <!--<glyph glyph-code="E8D4" smufl-name="accdnRicochetStem4"/>-->
            <!--<glyph glyph-code="E8D5" smufl-name="accdnRicochetStem5"/>-->
            <!--<glyph glyph-code="E8D6" smufl-name="accdnRicochetStem6"/>-->
            <range_end>U+E8DF</range_end>
            <range_start>U+E8A0</range_start>
        </accordion>
        <beamsAndSlurs>
            <description>Beams and slurs</description>
            <!--<glyph glyph-code="E8E0" smufl-name="controlBeginBeam"/>-->
            <!--<glyph glyph-code="E8E1" smufl-name="controlEndBeam"/>-->
            <!--<glyph glyph-code="E8E2" smufl-name="controlBeginTie"/>-->
            <!--<glyph glyph-code="E8E3" smufl-name="controlEndTie"/>-->
            <!--<glyph glyph-code="E8E4" smufl-name="controlBeginSlur"/>-->
            <!--<glyph glyph-code="E8E5" smufl-name="controlEndSlur"/>-->
            <!--<glyph glyph-code="E8E6" smufl-name="controlBeginPhrase"/>-->
            <!--<glyph glyph-code="E8E7" smufl-name="controlEndPhrase"/>-->
            <range_end>U+E8EF</range_end>
            <range_start>U+E8E0</range_start>
        </beamsAndSlurs>
        <medievalAndRenaissanceStaves>
            <description>Medieval and Renaissance staves</description>
            <!--<glyph glyph-code="E8F0" smufl-name="chantStaff"/>-->
            <!--<glyph glyph-code="E8F1" smufl-name="chantStaffWide"/>-->
            <!--<glyph glyph-code="E8F2" smufl-name="chantStaffNarrow"/>-->
            <!--<glyph glyph-code="E8F3" smufl-name="chantDivisioMinima"/>-->
            <!--<glyph glyph-code="E8F4" smufl-name="chantDivisioMaior"/>-->
            <!--<glyph glyph-code="E8F5" smufl-name="chantDivisioMaxima"/>-->
            <!--<glyph glyph-code="E8F6" smufl-name="chantDivisioFinalis"/>-->
            <!--<glyph glyph-code="E8F7" smufl-name="chantVirgula"/>-->
            <!--<glyph glyph-code="E8F8" smufl-name="chantCaesura"/>-->
            <range_end>U+E8FF</range_end>
            <range_start>U+E8F0</range_start>
        </medievalAndRenaissanceStaves>
        <medievalAndRenaissanceClefs>
            <description>Medieval and Renaissance clefs</description>
            <!--<glyph glyph-code="E900" smufl-name="mensuralGclef"/>-->
            <glyph glyph-code="E901" smufl-name="mensuralGclefPetrucci"/>
            <glyph glyph-code="E902" smufl-name="chantFclef"/>
            <!--<glyph glyph-code="E903" smufl-name="mensuralFclef"/>-->
            <glyph glyph-code="E904" smufl-name="mensuralFclefPetrucci"/>
            <!--<glyph glyph-code="E905" smufl-name="mensuralCclef"/>-->
            <glyph glyph-code="E906" smufl-name="chantCclef"/>
            <!--<glyph glyph-code="E907" smufl-name="mensuralCclefPetrucciPosLowest"/>-->
            <!--<glyph glyph-code="E908" smufl-name="mensuralCclefPetrucciPosLow"/>-->
            <glyph glyph-code="E909" smufl-name="mensuralCclefPetrucciPosMiddle"/>
            <!--<glyph glyph-code="E90A" smufl-name="mensuralCclefPetrucciPosHigh"/>-->
            <!--<glyph glyph-code="E90B" smufl-name="mensuralCclefPetrucciPosHighest"/>-->
            <range_end>U+E90F</range_end>
            <range_start>U+E900</range_start>
        </medievalAndRenaissanceClefs>
        <medievalAndRenaissanceProlations>
            <description>Medieval and Renaissance prolations</description>
            <glyph glyph-code="E910" smufl-name="mensuralProlation1"/>
            <glyph glyph-code="E911" smufl-name="mensuralProlation2"/>
            <glyph glyph-code="E912" smufl-name="mensuralProlation3"/>
            <glyph glyph-code="E913" smufl-name="mensuralProlation4"/>
            <glyph glyph-code="E914" smufl-name="mensuralProlation5"/>
            <glyph glyph-code="E915" smufl-name="mensuralProlation6"/>
            <glyph glyph-code="E916" smufl-name="mensuralProlation7"/>
            <glyph glyph-code="E917" smufl-name="mensuralProlation8"/>
            <glyph glyph-code="E918" smufl-name="mensuralProlation9"/>
            <glyph glyph-code="E919" smufl-name="mensuralProlation10"/>
            <glyph glyph-code="E91A" smufl-name="mensuralProlation11"/>
            <glyph glyph-code="E91B" smufl-name="mensuralProportionTempusPerfectum"/>
            <glyph glyph-code="E91C" smufl-name="mensuralProportionProportioDupla1"/>
            <glyph glyph-code="E91D" smufl-name="mensuralProportionProportioDupla2"/>
            <glyph glyph-code="E91E" smufl-name="mensuralProportionProportioTripla"/>
            <glyph glyph-code="E91F" smufl-name="mensuralProportionProportioQuadrupla"/>
            <glyph glyph-code="E920" smufl-name="mensuralProlationCombiningDot"/>
            <glyph glyph-code="E921" smufl-name="mensuralProlationCombiningTwoDots"/>
            <glyph glyph-code="E922" smufl-name="mensuralProlationCombiningThreeDots"/>
            <glyph glyph-code="E923" smufl-name="mensuralProlationCombiningThreeDotsTri"/>
            <glyph glyph-code="E924" smufl-name="mensuralProlationCombiningDotVoid"/>
            <glyph glyph-code="E925" smufl-name="mensuralProlationCombiningStroke"/>
            <!--<glyph glyph-code="E926" smufl-name="mensuralProportion1"/>-->
            <!--<glyph glyph-code="E927" smufl-name="mensuralProportion2"/>-->
            <!--<glyph glyph-code="E928" smufl-name="mensuralProportion3"/>-->
            <!--<glyph glyph-code="E929" smufl-name="mensuralProportion4"/>-->
            <!--<glyph glyph-code="E92A" smufl-name="mensuralProportionMinor"/>-->
            <!--<glyph glyph-code="E92B" smufl-name="mensuralProportionMajor"/>-->
            <!--<glyph glyph-code="E92C" smufl-name="mensuralModusPerfectumVert"/>-->
            <!--<glyph glyph-code="E92D" smufl-name="mensuralModusImperfectumVert"/>-->
            <!--<glyph glyph-code="E92E" smufl-name="mensuralTempusPerfectumHoriz"/>-->
            <!--<glyph glyph-code="E92F" smufl-name="mensuralTempusImperfectumHoriz"/>-->
            <range_end>U+E92F</range_end>
            <range_start>U+E910</range_start>
        </medievalAndRenaissanceProlations>
        <medievalAndRenaissanceNoteheadsAndStems>
            <description>Medieval and Renaissance noteheads and stems</description>
            <!--<glyph glyph-code="E930" smufl-name="mensuralNoteheadMaximaBlack"/>-->
            <!--<glyph glyph-code="E931" smufl-name="mensuralNoteheadMaximaVoid"/>-->
            <!--<glyph glyph-code="E932" smufl-name="mensuralNoteheadMaximaBlackVoid"/>-->
            <!--<glyph glyph-code="E933" smufl-name="mensuralNoteheadMaximaWhite"/>-->
            <!--<glyph glyph-code="E934" smufl-name="mensuralNoteheadLongaBlack"/>-->
            <!--<glyph glyph-code="E935" smufl-name="mensuralNoteheadLongaVoid"/>-->
            <!--<glyph glyph-code="E936" smufl-name="mensuralNoteheadLongaBlackVoid"/>-->
            <!--<glyph glyph-code="E937" smufl-name="mensuralNoteheadLongaWhite"/>-->
            <glyph glyph-code="E938" smufl-name="mensuralNoteheadSemibrevisBlack"/>
            <glyph glyph-code="E939" smufl-name="mensuralNoteheadSemibrevisVoid"/>
            <!--<glyph glyph-code="E93A" smufl-name="mensuralNoteheadSemibrevisBlackVoid"/>-->
            <!--<glyph glyph-code="E93B"
                smufl-name="mensuralNoteheadSemibrevisBlackVoidTurned"/>-->
            <glyph glyph-code="E93C" smufl-name="mensuralNoteheadMinimaWhite"/>
            <glyph glyph-code="E93D" smufl-name="mensuralNoteheadSemiminimaWhite"/>
            <glyph glyph-code="E93E" smufl-name="mensuralCombStemUp"/>
            <glyph glyph-code="E93F" smufl-name="mensuralCombStemDown"/>
            <!--<glyph glyph-code="E940" smufl-name="mensuralCombStemDiagonal"/>-->
            <!--<glyph glyph-code="E941" smufl-name="mensuralCombStemUpFlagRight"/>-->
            <!--<glyph glyph-code="E942" smufl-name="mensuralCombStemDownFlagRight"/>-->
            <!--<glyph glyph-code="E943" smufl-name="mensuralCombStemUpFlagLeft"/>-->
            <!--<glyph glyph-code="E944" smufl-name="mensuralCombStemDownFlagLeft"/>-->
            <!--<glyph glyph-code="E945" smufl-name="mensuralCombStemUpFlagFlared"/>-->
            <!--<glyph glyph-code="E946" smufl-name="mensuralCombStemDownFlagFlared"/>-->
            <!--<glyph glyph-code="E947" smufl-name="mensuralCombStemUpFlagExtended"/>-->
            <!--<glyph glyph-code="E948" smufl-name="mensuralCombStemDownFlagExtended"/>-->
            <glyph glyph-code="E949" smufl-name="mensuralCombStemUpFlagSemiminima"/>
            <glyph glyph-code="E94A" smufl-name="mensuralCombStemDownFlagSemiminima"/>
            <glyph glyph-code="E94B" smufl-name="mensuralCombStemUpFlagFusa"/>
            <glyph glyph-code="E94C" smufl-name="mensuralCombStemDownFlagFusa"/>
            <range_end>U+E94F</range_end>
            <range_start>U+E930</range_start>
        </medievalAndRenaissanceNoteheadsAndStems>
        <medievalAndRenaissanceIndividualNotes>
            <description>Medieval and Renaissance individual notes</description>
            <!--<glyph glyph-code="E950" smufl-name="mensuralBlackMaxima"/>-->
            <!--<glyph glyph-code="E951" smufl-name="mensuralBlackLonga"/>-->
            <!--<glyph glyph-code="E952" smufl-name="mensuralBlackBrevis"/>-->
            <!--<glyph glyph-code="E953" smufl-name="mensuralBlackSemibrevis"/>-->
            <!--<glyph glyph-code="E954" smufl-name="mensuralBlackMinima"/>-->
            <!--<glyph glyph-code="E955" smufl-name="mensuralBlackSemiminima"/>-->
            <!--<glyph glyph-code="E956" smufl-name="mensuralBlackBrevisVoid"/>-->
            <!--<glyph glyph-code="E957" smufl-name="mensuralBlackSemibrevisVoid"/>-->
            <!--<glyph glyph-code="E958" smufl-name="mensuralBlackMinimaVoid"/>-->
            <!--<glyph glyph-code="E959" smufl-name="mensuralBlackSemibrevisCaudata"/>-->
            <!--<glyph glyph-code="E95A" smufl-name="mensuralBlackDragma"/>-->
            <!--<glyph glyph-code="E95B" smufl-name="mensuralBlackSemibrevisOblique"/>-->
            <!--<glyph glyph-code="E95C" smufl-name="mensuralWhiteMaxima"/>-->
            <!--<glyph glyph-code="E95D" smufl-name="mensuralWhiteLonga"/>-->
            <!--<glyph glyph-code="E95E" smufl-name="mensuralWhiteBrevis"/>-->
            <!--<glyph glyph-code="E95F" smufl-name="mensuralWhiteMinima"/>-->
            <!--<glyph glyph-code="E960" smufl-name="mensuralWhiteSemiminima"/>-->
            <!--<glyph glyph-code="E961" smufl-name="mensuralWhiteFusa"/>-->
            <range_end>U+E96F</range_end>
            <range_start>U+E950</range_start>
        </medievalAndRenaissanceIndividualNotes>
        <medievalAndRenaissanceObliqueForms>
            <description>Medieval and Renaissance oblique forms</description>
            <!--<glyph glyph-code="E970" smufl-name="mensuralObliqueAsc2ndBlack"/>-->
            <!--<glyph glyph-code="E971" smufl-name="mensuralObliqueAsc2ndVoid"/>-->
            <!--<glyph glyph-code="E972" smufl-name="mensuralObliqueAsc2ndBlackVoid"/>-->
            <!--<glyph glyph-code="E973" smufl-name="mensuralObliqueAsc2ndWhite"/>-->
            <!--<glyph glyph-code="E974" smufl-name="mensuralObliqueAsc3rdBlack"/>-->
            <!--<glyph glyph-code="E975" smufl-name="mensuralObliqueAsc3rdVoid"/>-->
            <!--<glyph glyph-code="E976" smufl-name="mensuralObliqueAsc3rdBlackVoid"/>-->
            <!--<glyph glyph-code="E977" smufl-name="mensuralObliqueAsc3rdWhite"/>-->
            <!--<glyph glyph-code="E978" smufl-name="mensuralObliqueAsc4thBlack"/>-->
            <!--<glyph glyph-code="E979" smufl-name="mensuralObliqueAsc4thVoid"/>-->
            <!--<glyph glyph-code="E97A" smufl-name="mensuralObliqueAsc4thBlackVoid"/>-->
            <!--<glyph glyph-code="E97B" smufl-name="mensuralObliqueAsc4thWhite"/>-->
            <!--<glyph glyph-code="E97C" smufl-name="mensuralObliqueAsc5thBlack"/>-->
            <!--<glyph glyph-code="E97D" smufl-name="mensuralObliqueAsc5thVoid"/>-->
            <!--<glyph glyph-code="E97E" smufl-name="mensuralObliqueAsc5thBlackVoid"/>-->
            <!--<glyph glyph-code="E97F" smufl-name="mensuralObliqueAsc5thWhite"/>-->
            <!--<glyph glyph-code="E980" smufl-name="mensuralObliqueDesc2ndBlack"/>-->
            <!--<glyph glyph-code="E981" smufl-name="mensuralObliqueDesc2ndVoid"/>-->
            <!--<glyph glyph-code="E982" smufl-name="mensuralObliqueDesc2ndBlackVoid"/>-->
            <!--<glyph glyph-code="E983" smufl-name="mensuralObliqueDesc2ndWhite"/>-->
            <!--<glyph glyph-code="E984" smufl-name="mensuralObliqueDesc3rdBlack"/>-->
            <!--<glyph glyph-code="E985" smufl-name="mensuralObliqueDesc3rdVoid"/>-->
            <!--<glyph glyph-code="E986" smufl-name="mensuralObliqueDesc3rdBlackVoid"/>-->
            <!--<glyph glyph-code="E987" smufl-name="mensuralObliqueDesc3rdWhite"/>-->
            <!--<glyph glyph-code="E988" smufl-name="mensuralObliqueDesc4thBlack"/>-->
            <!--<glyph glyph-code="E989" smufl-name="mensuralObliqueDesc4thVoid"/>-->
            <!--<glyph glyph-code="E98A" smufl-name="mensuralObliqueDesc4thBlackVoid"/>-->
            <!--<glyph glyph-code="E98B" smufl-name="mensuralObliqueDesc4thWhite"/>-->
            <!--<glyph glyph-code="E98C" smufl-name="mensuralObliqueDesc5thBlack"/>-->
            <!--<glyph glyph-code="E98D" smufl-name="mensuralObliqueDesc5thVoid"/>-->
            <!--<glyph glyph-code="E98E" smufl-name="mensuralObliqueDesc5thBlackVoid"/>-->
            <!--<glyph glyph-code="E98F" smufl-name="mensuralObliqueDesc5thWhite"/>-->
            <range_end>U+E98F</range_end>
            <range_start>U+E970</range_start>
        </medievalAndRenaissanceObliqueForms>
        <medievalAndRenaissancePlainchantSingleNoteForms>
            <description>Medieval and Renaissance plainchant single-note forms</description>
            <glyph glyph-code="E990" smufl-name="chantPunctum"/>
            <glyph glyph-code="E991" smufl-name="chantPunctumInclinatum"/>
            <!--<glyph glyph-code="E992" smufl-name="chantPunctumInclinatumAuctum"/>-->
            <!--<glyph glyph-code="E993" smufl-name="chantPunctumInclinatumDeminutum"/>-->
            <!--<glyph glyph-code="E994" smufl-name="chantAuctumAsc"/>-->
            <!--<glyph glyph-code="E995" smufl-name="chantAuctumDesc"/>-->
            <glyph glyph-code="E996" smufl-name="chantPunctumVirga"/>
            <!--<glyph glyph-code="E997" smufl-name="chantPunctumVirgaReversed"/>-->
            <!--<glyph glyph-code="E998" smufl-name="chantPunctumCavum"/>-->
            <!--<glyph glyph-code="E999" smufl-name="chantPunctumLinea"/>-->
            <!--<glyph glyph-code="E99A" smufl-name="chantPunctumLineaCavum"/>-->
            <glyph glyph-code="E99B" smufl-name="chantQuilisma"/>
            <!--<glyph glyph-code="E99C" smufl-name="chantOriscusAscending"/>-->
            <!--<glyph glyph-code="E99D" smufl-name="chantOriscusDescending"/>-->
            <glyph glyph-code="E99E" smufl-name="chantOriscusLiquescens"/>
            <glyph glyph-code="E99F" smufl-name="chantStrophicus"/>
            <!--<glyph glyph-code="E9A0" smufl-name="chantStrophicusAuctus"/>-->
            <!--<glyph glyph-code="E9A1" smufl-name="chantPunctumDeminutum"/>-->
            <range_end>U+E9AF</range_end>
            <range_start>U+E990</range_start>
        </medievalAndRenaissancePlainchantSingleNoteForms>
        <medievalAndRenaissancePlainchantMultipleNoteForms>
            <description>Medieval and Renaissance plainchant multiple-note forms</description>
            <!--<glyph glyph-code="E9B0" smufl-name="chantPodatusLower"/>-->
            <!--<glyph glyph-code="E9B1" smufl-name="chantPodatusUpper"/>-->
            <!--<glyph glyph-code="E9B2" smufl-name="chantDeminutumUpper"/>-->
            <!--<glyph glyph-code="E9B3" smufl-name="chantDeminutumLower"/>-->
            <glyph glyph-code="E9B4" smufl-name="chantEntryLineAsc2nd"/>
            <glyph glyph-code="E9B5" smufl-name="chantEntryLineAsc3rd"/>
            <glyph glyph-code="E9B6" smufl-name="chantEntryLineAsc4th"/>
            <glyph glyph-code="E9B7" smufl-name="chantEntryLineAsc5th"/>
            <!--<glyph glyph-code="E9B8" smufl-name="chantEntryLineAsc6th"/>-->
            <glyph glyph-code="E9B9" smufl-name="chantLigaturaDesc2nd"/>
            <glyph glyph-code="E9BA" smufl-name="chantLigaturaDesc3rd"/>
            <glyph glyph-code="E9BB" smufl-name="chantLigaturaDesc4th"/>
            <glyph glyph-code="E9BC" smufl-name="chantLigaturaDesc5th"/>
            <!--<glyph glyph-code="E9BD" smufl-name="chantConnectingLineAsc2nd"/>-->
            <!--<glyph glyph-code="E9BE" smufl-name="chantConnectingLineAsc3rd"/>-->
            <!--<glyph glyph-code="E9BF" smufl-name="chantConnectingLineAsc4th"/>-->
            <!--<glyph glyph-code="E9C0" smufl-name="chantConnectingLineAsc5th"/>-->
            <!--<glyph glyph-code="E9C1" smufl-name="chantConnectingLineAsc6th"/>-->
            <!--<glyph glyph-code="E9C2" smufl-name="chantStrophicusLiquescens2nd"/>-->
            <!--<glyph glyph-code="E9C3" smufl-name="chantStrophicusLiquescens3rd"/>-->
            <!--<glyph glyph-code="E9C4" smufl-name="chantStrophicusLiquescens4th"/>-->
            <!--<glyph glyph-code="E9C5" smufl-name="chantStrophicusLiquescens5th"/>-->
            <range_end>U+E9CF</range_end>
            <range_start>U+E9B0</range_start>
        </medievalAndRenaissancePlainchantMultipleNoteForms>
        <medievalAndRenaissancePlainchantArticulations>
            <description>Medieval and Renaissance plainchant articulations</description>
            <!--<glyph glyph-code="E9D0" smufl-name="chantIctusAbove"/>-->
            <!--<glyph glyph-code="E9D1" smufl-name="chantIctusBelow"/>-->
            <!--<glyph glyph-code="E9D2" smufl-name="chantCirculusAbove"/>-->
            <!--<glyph glyph-code="E9D3" smufl-name="chantCirculusBelow"/>-->
            <!--<glyph glyph-code="E9D4" smufl-name="chantSemicirculusAbove"/>-->
            <!--<glyph glyph-code="E9D5" smufl-name="chantSemicirculusBelow"/>-->
            <!--<glyph glyph-code="E9D6" smufl-name="chantAccentusAbove"/>-->
            <!--<glyph glyph-code="E9D7" smufl-name="chantAccentusBelow"/>-->
            <!--<glyph glyph-code="E9D8" smufl-name="chantEpisema"/>-->
            <!--<glyph glyph-code="E9D9" smufl-name="chantAugmentum"/>-->
            <range_end>U+E9DF</range_end>
            <range_start>U+E9D0</range_start>
        </medievalAndRenaissancePlainchantArticulations>
        <medievalAndRenaissanceAccidentals>
            <description>Medieval and Renaissance accidentals</description>
            <!--<glyph glyph-code="E9E0" smufl-name="medRenFlatSoftB"/>-->
            <!--<glyph glyph-code="E9E1" smufl-name="medRenFlatHardB"/>-->
            <!--<glyph glyph-code="E9E2" smufl-name="medRenNatural"/>-->
            <!--<glyph glyph-code="E9E3" smufl-name="medRenSharpCroix"/>-->
            <!--<glyph glyph-code="E9E4" smufl-name="medRenFlatWithDot"/>-->
            <!--<glyph glyph-code="E9E5" smufl-name="medRenNaturalWithCross"/>-->
            <range_end>U+E9EF</range_end>
            <range_start>U+E9E0</range_start>
        </medievalAndRenaissanceAccidentals>
        <medievalAndRenaissanceRests>
            <description>Medieval and Renaissance rests</description>
            <glyph glyph-code="E9F0" smufl-name="mensuralRestMaxima"/>
            <glyph glyph-code="E9F1" smufl-name="mensuralRestLongaPerfecta"/>
            <glyph glyph-code="E9F2" smufl-name="mensuralRestLongaImperfecta"/>
            <glyph glyph-code="E9F3" smufl-name="mensuralRestBrevis"/>
            <glyph glyph-code="E9F4" smufl-name="mensuralRestSemibrevis"/>
            <glyph glyph-code="E9F5" smufl-name="mensuralRestMinima"/>
            <glyph glyph-code="E9F6" smufl-name="mensuralRestSemiminima"/>
            <glyph glyph-code="E9F7" smufl-name="mensuralRestFusa"/>
            <glyph glyph-code="E9F8" smufl-name="mensuralRestSemifusa"/>
            <range_end>U+E9FF</range_end>
            <range_start>U+E9F0</range_start>
        </medievalAndRenaissanceRests>
        <medievalAndRenaissanceMiscellany>
            <description>Medieval and Renaissance miscellany</description>
            <!--<glyph glyph-code="EA00" smufl-name="mensuralSignumUp"/>-->
            <!--<glyph glyph-code="EA01" smufl-name="mensuralSignumDown"/>-->
            <glyph glyph-code="EA02" smufl-name="mensuralCustosUp"/>
            <!--<glyph glyph-code="EA03" smufl-name="mensuralCustosDown"/>-->
            <!--<glyph glyph-code="EA04" smufl-name="chantCustosStemUpPosLowest"/>-->
            <!--<glyph glyph-code="EA05" smufl-name="chantCustosStemUpPosLow"/>-->
            <glyph glyph-code="EA06" smufl-name="chantCustosStemUpPosMiddle"/>
            <!--<glyph glyph-code="EA07" smufl-name="chantCustosStemDownPosMiddle"/>-->
            <!--<glyph glyph-code="EA08" smufl-name="chantCustosStemDownPosHigh"/>-->
            <!--<glyph glyph-code="EA09" smufl-name="chantCustosStemDownPosHighest"/>-->
            <!--<glyph glyph-code="EA0A" smufl-name="mensuralCustosCheckmark"/>-->
            <!--<glyph glyph-code="EA0B" smufl-name="mensuralCustosTurn"/>-->
            <!--<glyph glyph-code="EA0C" smufl-name="mensuralColorationStartSquare"/>-->
            <!--<glyph glyph-code="EA0D" smufl-name="mensuralColorationEndSquare"/>-->
            <!--<glyph glyph-code="EA0E" smufl-name="mensuralColorationStartRound"/>-->
            <!--<glyph glyph-code="EA0F" smufl-name="mensuralColorationEndRound"/>-->
            <!--<glyph glyph-code="EA10" smufl-name="mensuralAlterationSign"/>-->
            <range_end>U+EA1F</range_end>
            <range_start>U+EA00</range_start>
        </medievalAndRenaissanceMiscellany>
        <medievalAndRenaissanceSymbolsInCmn>
            <description>Medieval and Renaissance symbols in CMN</description>
            <!--<glyph glyph-code="EA20" smufl-name="ornamentQuilisma"/>-->
            <!--<glyph glyph-code="EA21" smufl-name="ornamentOriscus"/>-->
            <!--<glyph glyph-code="EA22" smufl-name="medRenLiquescenceCMN"/>-->
            <!--<glyph glyph-code="EA23" smufl-name="medRenPlicaCMN"/>-->
            <!--<glyph glyph-code="EA24" smufl-name="medRenGClefCMN"/>-->
            <!--<glyph glyph-code="EA25" smufl-name="medRenPunctumCMN"/>-->
            <!--<glyph glyph-code="EA26" smufl-name="medRenLiquescentAscCMN"/>-->
            <!--<glyph glyph-code="EA27" smufl-name="medRenLiquescentDescCMN"/>-->
            <!--<glyph glyph-code="EA28" smufl-name="medRenQuilismaCMN"/>-->
            <!--<glyph glyph-code="EA29" smufl-name="medRenStrophicusCMN"/>-->
            <!--<glyph glyph-code="EA2A" smufl-name="medRenOriscusCMN"/>-->
            <range_end>U+EA2F</range_end>
            <range_start>U+EA20</range_start>
        </medievalAndRenaissanceSymbolsInCmn>
        <daseianNotation>
            <description>Daseian notation</description>
            <!--<glyph glyph-code="EA30" smufl-name="daseianGraves1"/>-->
            <!--<glyph glyph-code="EA31" smufl-name="daseianGraves2"/>-->
            <!--<glyph glyph-code="EA32" smufl-name="daseianGraves3"/>-->
            <!--<glyph glyph-code="EA33" smufl-name="daseianGraves4"/>-->
            <!--<glyph glyph-code="EA34" smufl-name="daseianFinales1"/>-->
            <!--<glyph glyph-code="EA35" smufl-name="daseianFinales2"/>-->
            <!--<glyph glyph-code="EA36" smufl-name="daseianFinales3"/>-->
            <!--<glyph glyph-code="EA37" smufl-name="daseianFinales4"/>-->
            <!--<glyph glyph-code="EA38" smufl-name="daseianSuperiores1"/>-->
            <!--<glyph glyph-code="EA39" smufl-name="daseianSuperiores2"/>-->
            <!--<glyph glyph-code="EA3A" smufl-name="daseianSuperiores3"/>-->
            <!--<glyph glyph-code="EA3B" smufl-name="daseianSuperiores4"/>-->
            <!--<glyph glyph-code="EA3C" smufl-name="daseianExcellentes1"/>-->
            <!--<glyph glyph-code="EA3D" smufl-name="daseianExcellentes2"/>-->
            <!--<glyph glyph-code="EA3E" smufl-name="daseianExcellentes3"/>-->
            <!--<glyph glyph-code="EA3F" smufl-name="daseianExcellentes4"/>-->
            <!--<glyph glyph-code="EA40" smufl-name="daseianResidua1"/>-->
            <!--<glyph glyph-code="EA41" smufl-name="daseianResidua2"/>-->
            <range_end>U+EA4F</range_end>
            <range_start>U+EA30</range_start>
        </daseianNotation>
        <figuredBass>
            <description>Figured bass</description>
            <!--<glyph glyph-code="EA50" smufl-name="figbass0"/>-->
            <!--<glyph glyph-code="EA51" smufl-name="figbass1"/>-->
            <!--<glyph glyph-code="EA52" smufl-name="figbass2"/>-->
            <!--<glyph glyph-code="EA53" smufl-name="figbass2Raised"/>-->
            <!--<glyph glyph-code="EA54" smufl-name="figbass3"/>-->
            <!--<glyph glyph-code="EA55" smufl-name="figbass4"/>-->
            <!--<glyph glyph-code="EA56" smufl-name="figbass4Raised"/>-->
            <!--<glyph glyph-code="EA57" smufl-name="figbass5"/>-->
            <!--<glyph glyph-code="EA58" smufl-name="figbass5Raised1"/>-->
            <!--<glyph glyph-code="EA59" smufl-name="figbass5Raised2"/>-->
            <!--<glyph glyph-code="EA5A" smufl-name="figbass5Raised3"/>-->
            <!--<glyph glyph-code="EA5B" smufl-name="figbass6"/>-->
            <!--<glyph glyph-code="EA5C" smufl-name="figbass6Raised"/>-->
            <!--<glyph glyph-code="EA5D" smufl-name="figbass7"/>-->
            <!--<glyph glyph-code="EA5E" smufl-name="figbass7Raised1"/>-->
            <!--<glyph glyph-code="EA5F" smufl-name="figbass7Raised2"/>-->
            <!--<glyph glyph-code="EA60" smufl-name="figbass8"/>-->
            <!--<glyph glyph-code="EA61" smufl-name="figbass9"/>-->
            <!--<glyph glyph-code="EA62" smufl-name="figbass9Raised"/>-->
            <!--<glyph glyph-code="EA63" smufl-name="figbassDoubleFlat"/>-->
            <!--<glyph glyph-code="EA64" smufl-name="figbassFlat"/>-->
            <!--<glyph glyph-code="EA65" smufl-name="figbassNatural"/>-->
            <!--<glyph glyph-code="EA66" smufl-name="figbassSharp"/>-->
            <!--<glyph glyph-code="EA67" smufl-name="figbassDoubleSharp"/>-->
            <!--<glyph glyph-code="EA68" smufl-name="figbassBracketLeft"/>-->
            <!--<glyph glyph-code="EA69" smufl-name="figbassBracketRight"/>-->
            <!--<glyph glyph-code="EA6A" smufl-name="figbassParensLeft"/>-->
            <!--<glyph glyph-code="EA6B" smufl-name="figbassParensRight"/>-->
            <!--<glyph glyph-code="EA6C" smufl-name="figbassPlus"/>-->
            <!--<glyph glyph-code="EA6D" smufl-name="figbassCombiningRaising"/>-->
            <!--<glyph glyph-code="EA6E" smufl-name="figbassCombiningLowering"/>-->
            <range_end>U+EA6F</range_end>
            <range_start>U+EA50</range_start>
        </figuredBass>
        <functionTheorySymbols>
            <description>Function theory symbols</description>
            <!--<glyph glyph-code="EA70" smufl-name="functionZero"/>-->
            <!--<glyph glyph-code="EA71" smufl-name="functionOne"/>-->
            <!--<glyph glyph-code="EA72" smufl-name="functionTwo"/>-->
            <!--<glyph glyph-code="EA73" smufl-name="functionThree"/>-->
            <!--<glyph glyph-code="EA74" smufl-name="functionFour"/>-->
            <!--<glyph glyph-code="EA75" smufl-name="functionFive"/>-->
            <!--<glyph glyph-code="EA76" smufl-name="functionSix"/>-->
            <!--<glyph glyph-code="EA77" smufl-name="functionSeven"/>-->
            <!--<glyph glyph-code="EA78" smufl-name="functionEight"/>-->
            <!--<glyph glyph-code="EA79" smufl-name="functionNine"/>-->
            <!--<glyph glyph-code="EA7A" smufl-name="functionLessThan"/>-->
            <!--<glyph glyph-code="EA7B" smufl-name="functionMinus"/>-->
            <!--<glyph glyph-code="EA7C" smufl-name="functionGreaterThan"/>-->
            <!--<glyph glyph-code="EA7D" smufl-name="functionSSUpper"/>-->
            <!--<glyph glyph-code="EA7E" smufl-name="functionSSLower"/>-->
            <!--<glyph glyph-code="EA7F" smufl-name="functionDUpper"/>-->
            <!--<glyph glyph-code="EA80" smufl-name="functionDLower"/>-->
            <!--<glyph glyph-code="EA81" smufl-name="functionDD"/>-->
            <!--<glyph glyph-code="EA82" smufl-name="functionSlashedDD"/>-->
            <!--<glyph glyph-code="EA83" smufl-name="functionGUpper"/>-->
            <!--<glyph glyph-code="EA84" smufl-name="functionGLower"/>-->
            <!--<glyph glyph-code="EA85" smufl-name="functionNUpper"/>-->
            <!--<glyph glyph-code="EA86" smufl-name="functionNLower"/>-->
            <!--<glyph glyph-code="EA87" smufl-name="functionPUpper"/>-->
            <!--<glyph glyph-code="EA88" smufl-name="functionPLower"/>-->
            <!--<glyph glyph-code="EA89" smufl-name="functionSUpper"/>-->
            <!--<glyph glyph-code="EA8A" smufl-name="functionSLower"/>-->
            <!--<glyph glyph-code="EA8B" smufl-name="functionTUpper"/>-->
            <!--<glyph glyph-code="EA8C" smufl-name="functionTLower"/>-->
            <!--<glyph glyph-code="EA8D" smufl-name="functionVUpper"/>-->
            <!--<glyph glyph-code="EA8E" smufl-name="functionVLower"/>-->
            <!--<glyph glyph-code="EA8F" smufl-name="functionBracketLeft"/>-->
            <!--<glyph glyph-code="EA90" smufl-name="functionBracketRight"/>-->
            <!--<glyph glyph-code="EA91" smufl-name="functionParensLeft"/>-->
            <!--<glyph glyph-code="EA92" smufl-name="functionParensRight"/>-->
            <!--<glyph glyph-code="EA93" smufl-name="functionAngleLeft"/>-->
            <!--<glyph glyph-code="EA94" smufl-name="functionAngleRight"/>-->
            <!--<glyph glyph-code="EA95" smufl-name="functionRepetition1"/>-->
            <!--<glyph glyph-code="EA96" smufl-name="functionRepetition2"/>-->
            <!--<glyph glyph-code="EA97" smufl-name="functionRing"/>-->
            <!--<glyph glyph-code="EA98" smufl-name="functionPlus"/>-->
            <range_end>U+EA9F</range_end>
            <range_start>U+EA70</range_start>
        </functionTheorySymbols>
        <multiSegmentLines>
            <description>Multi-segment lines</description>
            <!--<glyph glyph-code="EAA0" smufl-name="wiggleTrillFastest"/>-->
            <!--<glyph glyph-code="EAA1" smufl-name="wiggleTrillFasterStill"/>-->
            <!--<glyph glyph-code="EAA2" smufl-name="wiggleTrillFaster"/>-->
            <!--<glyph glyph-code="EAA3" smufl-name="wiggleTrillFast"/>-->
            <!--<glyph glyph-code="EAA4" smufl-name="wiggleTrill"/>-->
            <!--<glyph glyph-code="EAA5" smufl-name="wiggleTrillSlow"/>-->
            <!--<glyph glyph-code="EAA6" smufl-name="wiggleTrillSlower"/>-->
            <!--<glyph glyph-code="EAA7" smufl-name="wiggleTrillSlowerStill"/>-->
            <!--<glyph glyph-code="EAA8" smufl-name="wiggleTrillSlowest"/>-->
            <glyph glyph-code="EAA9" smufl-name="wiggleArpeggiatoUp"/>
            <glyph glyph-code="EAAA" smufl-name="wiggleArpeggiatoDown"/>
            <!--<glyph glyph-code="EAAB" smufl-name="wiggleArpeggiatoUpSwash"/>-->
            <!--<glyph glyph-code="EAAC" smufl-name="wiggleArpeggiatoDownSwash"/>-->
            <glyph glyph-code="EAAD" smufl-name="wiggleArpeggiatoUpArrow"/>
            <glyph glyph-code="EAAE" smufl-name="wiggleArpeggiatoDownArrow"/>
            <!--<glyph glyph-code="EAAF" smufl-name="wiggleGlissando"/>-->
            <!--<glyph glyph-code="EAB0" smufl-name="wiggleVibrato"/>-->
            <!--<glyph glyph-code="EAB1" smufl-name="wiggleVibratoWide"/>-->
            <!--<glyph glyph-code="EAB2" smufl-name="guitarVibratoStroke"/>-->
            <!--<glyph glyph-code="EAB3" smufl-name="guitarWideVibratoStroke"/>-->
            <!--<glyph glyph-code="EAB4" smufl-name="wiggleWavyNarrow"/>-->
            <!--<glyph glyph-code="EAB5" smufl-name="wiggleWavy"/>-->
            <!--<glyph glyph-code="EAB6" smufl-name="wiggleWavyWide"/>-->
            <!--<glyph glyph-code="EAB7" smufl-name="wiggleSquareWaveNarrow"/>-->
            <!--<glyph glyph-code="EAB8" smufl-name="wiggleSquareWave"/>-->
            <!--<glyph glyph-code="EAB9" smufl-name="wiggleSquareWaveWide"/>-->
            <!--<glyph glyph-code="EABA" smufl-name="wiggleSawtoothNarrow"/>-->
            <!--<glyph glyph-code="EABB" smufl-name="wiggleSawtooth"/>-->
            <!--<glyph glyph-code="EABC" smufl-name="wiggleSawtoothWide"/>-->
            <!--<glyph glyph-code="EABD" smufl-name="wiggleGlissandoGroup1"/>-->
            <!--<glyph glyph-code="EABE" smufl-name="wiggleGlissandoGroup2"/>-->
            <!--<glyph glyph-code="EABF" smufl-name="wiggleGlissandoGroup3"/>-->
            <!--<glyph glyph-code="EAC0" smufl-name="wiggleCircularConstant"/>-->
            <!--<glyph glyph-code="EAC1" smufl-name="wiggleCircularConstantFlipped"/>-->
            <!--<glyph glyph-code="EAC2" smufl-name="wiggleCircularConstantLarge"/>-->
            <!--<glyph glyph-code="EAC3" smufl-name="wiggleCircularConstantFlippedLarge"/>-->
            <!--<glyph glyph-code="EAC4" smufl-name="wiggleCircularStart"/>-->
            <!--<glyph glyph-code="EAC5" smufl-name="wiggleCircularLargest"/>-->
            <!--<glyph glyph-code="EAC6" smufl-name="wiggleCircularLargerStill"/>-->
            <!--<glyph glyph-code="EAC7" smufl-name="wiggleCircularLarger"/>-->
            <!--<glyph glyph-code="EAC8" smufl-name="wiggleCircularLarge"/>-->
            <!--<glyph glyph-code="EAC9" smufl-name="wiggleCircular"/>-->
            <!--<glyph glyph-code="EACA" smufl-name="wiggleCircularSmall"/>-->
            <!--<glyph glyph-code="EACB" smufl-name="wiggleCircularEnd"/>-->
            <!--<glyph glyph-code="EACC" smufl-name="wiggleVibratoStart"/>-->
            <!--<glyph glyph-code="EACD" smufl-name="wiggleVibratoSmallestFastest"/>-->
            <!--<glyph glyph-code="EACE" smufl-name="wiggleVibratoSmallestFasterStill"/>-->
            <!--<glyph glyph-code="EACF" smufl-name="wiggleVibratoSmallestFaster"/>-->
            <!--<glyph glyph-code="EAD0" smufl-name="wiggleVibratoSmallestFast"/>-->
            <!--<glyph glyph-code="EAD1" smufl-name="wiggleVibratoSmallestSlow"/>-->
            <!--<glyph glyph-code="EAD2" smufl-name="wiggleVibratoSmallestSlower"/>-->
            <!--<glyph glyph-code="EAD3" smufl-name="wiggleVibratoSmallestSlowest"/>-->
            <!--<glyph glyph-code="EAD4" smufl-name="wiggleVibratoSmallFastest"/>-->
            <!--<glyph glyph-code="EAD5" smufl-name="wiggleVibratoSmallFasterStill"/>-->
            <!--<glyph glyph-code="EAD6" smufl-name="wiggleVibratoSmallFaster"/>-->
            <!--<glyph glyph-code="EAD7" smufl-name="wiggleVibratoSmallFast"/>-->
            <!--<glyph glyph-code="EAD8" smufl-name="wiggleVibratoSmallSlow"/>-->
            <!--<glyph glyph-code="EAD9" smufl-name="wiggleVibratoSmallSlower"/>-->
            <!--<glyph glyph-code="EADA" smufl-name="wiggleVibratoSmallSlowest"/>-->
            <!--<glyph glyph-code="EADB" smufl-name="wiggleVibratoMediumFastest"/>-->
            <!--<glyph glyph-code="EADC" smufl-name="wiggleVibratoMediumFasterStill"/>-->
            <!--<glyph glyph-code="EADD" smufl-name="wiggleVibratoMediumFaster"/>-->
            <!--<glyph glyph-code="EADE" smufl-name="wiggleVibratoMediumFast"/>-->
            <!--<glyph glyph-code="EADF" smufl-name="wiggleVibratoMediumSlow"/>-->
            <!--<glyph glyph-code="EAE0" smufl-name="wiggleVIbratoMediumSlower"/>-->
            <!--<glyph glyph-code="EAE1" smufl-name="wiggleVibratoMediumSlowest"/>-->
            <!--<glyph glyph-code="EAE2" smufl-name="wiggleVibratoLargeFastest"/>-->
            <!--<glyph glyph-code="EAE3" smufl-name="wiggleVibratoLargeFasterStill"/>-->
            <!--<glyph glyph-code="EAE4" smufl-name="wiggleVibratoLargeFaster"/>-->
            <!--<glyph glyph-code="EAE5" smufl-name="wiggleVibratoLargeFast"/>-->
            <!--<glyph glyph-code="EAE6" smufl-name="wiggleVibratoLargeSlow"/>-->
            <!--<glyph glyph-code="EAE7" smufl-name="wiggleVibratoLargeSlower"/>-->
            <!--<glyph glyph-code="EAE8" smufl-name="wiggleVibratoLargeSlowest"/>-->
            <!--<glyph glyph-code="EAE9" smufl-name="wiggleVibratoLargestFastest"/>-->
            <!--<glyph glyph-code="EAEA" smufl-name="wiggleVibratoLargestFasterStill"/>-->
            <!--<glyph glyph-code="EAEB" smufl-name="wiggleVibratoLargestFaster"/>-->
            <!--<glyph glyph-code="EAEC" smufl-name="wiggleVibratoLargestFast"/>-->
            <!--<glyph glyph-code="EAED" smufl-name="wiggleVibratoLargestSlow"/>-->
            <!--<glyph glyph-code="EAEE" smufl-name="wiggleVIbratoLargestSlower"/>-->
            <!--<glyph glyph-code="EAEF" smufl-name="wiggleVibratoLargestSlowest"/>-->
            <!--<glyph glyph-code="EAF0" smufl-name="wiggleRandom1"/>-->
            <!--<glyph glyph-code="EAF1" smufl-name="wiggleRandom2"/>-->
            <!--<glyph glyph-code="EAF2" smufl-name="wiggleRandom3"/>-->
            <!--<glyph glyph-code="EAF3" smufl-name="wiggleRandom4"/>-->
            <!--<glyph glyph-code="EAF4" smufl-name="beamAccelRit1"/>-->
            <!--<glyph glyph-code="EAF5" smufl-name="beamAccelRit2"/>-->
            <!--<glyph glyph-code="EAF6" smufl-name="beamAccelRit3"/>-->
            <!--<glyph glyph-code="EAF7" smufl-name="beamAccelRit4"/>-->
            <!--<glyph glyph-code="EAF8" smufl-name="beamAccelRit5"/>-->
            <!--<glyph glyph-code="EAF9" smufl-name="beamAccelRit6"/>-->
            <!--<glyph glyph-code="EAFA" smufl-name="beamAccelRit7"/>-->
            <!--<glyph glyph-code="EAFB" smufl-name="beamAccelRit8"/>-->
            <!--<glyph glyph-code="EAFC" smufl-name="beamAccelRit9"/>-->
            <!--<glyph glyph-code="EAFD" smufl-name="beamAccelRit10"/>-->
            <!--<glyph glyph-code="EAFE" smufl-name="beamAccelRit11"/>-->
            <!--<glyph glyph-code="EAFF" smufl-name="beamAccelRit12"/>-->
            <!--<glyph glyph-code="EB00" smufl-name="beamAccelRit13"/>-->
            <!--<glyph glyph-code="EB01" smufl-name="beamAccelRit14"/>-->
            <!--<glyph glyph-code="EB02" smufl-name="beamAccelRit15"/>-->
            <!--<glyph glyph-code="EB03" smufl-name="beamAccelRitFinal"/>-->
            <range_end>U+EB0F</range_end>
            <range_start>U+EAA0</range_start>
        </multiSegmentLines>
        <electronicMusicPictograms>
            <description>Electronic music pictograms</description>
            <!--<glyph glyph-code="EB10" smufl-name="elecMicrophone"/>-->
            <!--<glyph glyph-code="EB11" smufl-name="elecHeadphones"/>-->
            <!--<glyph glyph-code="EB12" smufl-name="elecHeadset"/>-->
            <!--<glyph glyph-code="EB13" smufl-name="elecDisc"/>-->
            <!--<glyph glyph-code="EB14" smufl-name="elecTape"/>-->
            <!--<glyph glyph-code="EB15" smufl-name="elecMixingConsole"/>-->
            <!--<glyph glyph-code="EB16" smufl-name="elecUSB"/>-->
            <!--<glyph glyph-code="EB17" smufl-name="elecVideoCamera"/>-->
            <!--<glyph glyph-code="EB18" smufl-name="elecMonitor"/>-->
            <!--<glyph glyph-code="EB19" smufl-name="elecProjector"/>-->
            <!--<glyph glyph-code="EB1A" smufl-name="elecLoudspeaker"/>-->
            <!--<glyph glyph-code="EB1B" smufl-name="elecCamera"/>-->
            <!--<glyph glyph-code="EB1C" smufl-name="elecPlay"/>-->
            <!--<glyph glyph-code="EB1D" smufl-name="elecStop"/>-->
            <!--<glyph glyph-code="EB1E" smufl-name="elecPause"/>-->
            <!--<glyph glyph-code="EB1F" smufl-name="elecFastForward"/>-->
            <!--<glyph glyph-code="EB20" smufl-name="elecRewind"/>-->
            <!--<glyph glyph-code="EB21" smufl-name="elecSkipForwards"/>-->
            <!--<glyph glyph-code="EB22" smufl-name="elecSkipBackwards"/>-->
            <!--<glyph glyph-code="EB23" smufl-name="elecLoop"/>-->
            <!--<glyph glyph-code="EB24" smufl-name="elecReplay"/>-->
            <!--<glyph glyph-code="EB25" smufl-name="elecShuffle"/>-->
            <!--<glyph glyph-code="EB26" smufl-name="elecMute"/>-->
            <!--<glyph glyph-code="EB27" smufl-name="elecUnmute"/>-->
            <!--<glyph glyph-code="EB28" smufl-name="elecMicrophoneMute"/>-->
            <!--<glyph glyph-code="EB29" smufl-name="elecMicrophoneUnmute"/>-->
            <!--<glyph glyph-code="EB2A" smufl-name="elecPowerOnOff"/>-->
            <!--<glyph glyph-code="EB2B" smufl-name="elecEject"/>-->
            <!--<glyph glyph-code="EB2C" smufl-name="elecVolumeFader"/>-->
            <!--<glyph glyph-code="EB2D" smufl-name="elecVolumeFaderThumb"/>-->
            <!--<glyph glyph-code="EB2E" smufl-name="elecVolumeLevel0"/>-->
            <!--<glyph glyph-code="EB2F" smufl-name="elecVolumeLevel20"/>-->
            <!--<glyph glyph-code="EB30" smufl-name="elecVolumeLevel40"/>-->
            <!--<glyph glyph-code="EB31" smufl-name="elecVolumeLevel60"/>-->
            <!--<glyph glyph-code="EB32" smufl-name="elecVolumeLevel80"/>-->
            <!--<glyph glyph-code="EB33" smufl-name="elecVolumeLevel100"/>-->
            <!--<glyph glyph-code="EB34" smufl-name="elecMIDIIn"/>-->
            <!--<glyph glyph-code="EB35" smufl-name="elecMIDIOut"/>-->
            <!--<glyph glyph-code="EB36" smufl-name="elecMIDIController0"/>-->
            <!--<glyph glyph-code="EB37" smufl-name="elecMIDIController20"/>-->
            <!--<glyph glyph-code="EB38" smufl-name="elecMIDIController40"/>-->
            <!--<glyph glyph-code="EB39" smufl-name="elecMIDIController60"/>-->
            <!--<glyph glyph-code="EB3A" smufl-name="elecMIDIController80"/>-->
            <!--<glyph glyph-code="EB3B" smufl-name="elecMIDIController100"/>-->
            <!--<glyph glyph-code="EB3C" smufl-name="elecAudioMono"/>-->
            <!--<glyph glyph-code="EB3D" smufl-name="elecAudioStereo"/>-->
            <!--<glyph glyph-code="EB3E" smufl-name="elecAudioChannelsOne"/>-->
            <!--<glyph glyph-code="EB3F" smufl-name="elecAudioChannelsTwo"/>-->
            <!--<glyph glyph-code="EB40" smufl-name="elecAudioChannelsThreeFrontal"/>-->
            <!--<glyph glyph-code="EB41" smufl-name="elecAudioChannelsThreeSurround"/>-->
            <!--<glyph glyph-code="EB42" smufl-name="elecAudioChannelsFour"/>-->
            <!--<glyph glyph-code="EB43" smufl-name="elecAudioChannelsFive"/>-->
            <!--<glyph glyph-code="EB44" smufl-name="elecAudioChannelsSix"/>-->
            <!--<glyph glyph-code="EB45" smufl-name="elecAudioChannelsSeven"/>-->
            <!--<glyph glyph-code="EB46" smufl-name="elecAudioChannelsEight"/>-->
            <!--<glyph glyph-code="EB47" smufl-name="elecLineIn"/>-->
            <!--<glyph glyph-code="EB48" smufl-name="elecLineOut"/>-->
            <!--<glyph glyph-code="EB49" smufl-name="elecAudioIn"/>-->
            <!--<glyph glyph-code="EB4A" smufl-name="elecAudioOut"/>-->
            <!--<glyph glyph-code="EB4B" smufl-name="elecVideoIn"/>-->
            <!--<glyph glyph-code="EB4C" smufl-name="elecVideoOut"/>-->
            <!--<glyph glyph-code="EB4D" smufl-name="elecDataIn"/>-->
            <!--<glyph glyph-code="EB4E" smufl-name="elecDataOut"/>-->
            <!--<glyph glyph-code="EB4F" smufl-name="elecDownload"/>-->
            <!--<glyph glyph-code="EB50" smufl-name="elecUpload"/>-->
            <range_end>U+EB5F</range_end>
            <range_start>U+EB10</range_start>
        </electronicMusicPictograms>
        <arrowsAndArrowheads>
            <description>Arrows and arrowheads</description>
            <!--<glyph glyph-code="EB60" smufl-name="arrowBlackUp"/>-->
            <!--<glyph glyph-code="EB61" smufl-name="arrowBlackUpRight"/>-->
            <!--<glyph glyph-code="EB62" smufl-name="arrowBlackRight"/>-->
            <!--<glyph glyph-code="EB63" smufl-name="arrowBlackDownRight"/>-->
            <!--<glyph glyph-code="EB64" smufl-name="arrowBlackDown"/>-->
            <!--<glyph glyph-code="EB65" smufl-name="arrowBlackDownLeft"/>-->
            <!--<glyph glyph-code="EB66" smufl-name="arrowBlackLeft"/>-->
            <!--<glyph glyph-code="EB67" smufl-name="arrowBlackUpLeft"/>-->
            <!--<glyph glyph-code="EB68" smufl-name="arrowWhiteUp"/>-->
            <!--<glyph glyph-code="EB69" smufl-name="arrowWhiteUpRight"/>-->
            <!--<glyph glyph-code="EB6A" smufl-name="arrowWhiteRight"/>-->
            <!--<glyph glyph-code="EB6B" smufl-name="arrowWhiteDownRight"/>-->
            <!--<glyph glyph-code="EB6C" smufl-name="arrowWhiteDown"/>-->
            <!--<glyph glyph-code="EB6D" smufl-name="arrowWhiteDownLeft"/>-->
            <!--<glyph glyph-code="EB6E" smufl-name="arrowWhiteLeft"/>-->
            <!--<glyph glyph-code="EB6F" smufl-name="arrowWhiteUpLeft"/>-->
            <!--<glyph glyph-code="EB70" smufl-name="arrowOpenUp"/>-->
            <!--<glyph glyph-code="EB71" smufl-name="arrowOpenUpRight"/>-->
            <!--<glyph glyph-code="EB72" smufl-name="arrowOpenRight"/>-->
            <!--<glyph glyph-code="EB73" smufl-name="arrowOpenDownRight"/>-->
            <!--<glyph glyph-code="EB74" smufl-name="arrowOpenDown"/>-->
            <!--<glyph glyph-code="EB75" smufl-name="arrowOpenDownLeft"/>-->
            <!--<glyph glyph-code="EB76" smufl-name="arrowOpenLeft"/>-->
            <!--<glyph glyph-code="EB77" smufl-name="arrowOpenUpLeft"/>-->
            <!--<glyph glyph-code="EB78" smufl-name="arrowheadBlackUp"/>-->
            <!--<glyph glyph-code="EB79" smufl-name="arrowheadBlackUpRight"/>-->
            <!--<glyph glyph-code="EB7A" smufl-name="arrowheadBlackRight"/>-->
            <!--<glyph glyph-code="EB7B" smufl-name="arrowheadBlackDownRight"/>-->
            <!--<glyph glyph-code="EB7C" smufl-name="arrowheadBlackDown"/>-->
            <!--<glyph glyph-code="EB7D" smufl-name="arrowheadBlackDownLeft"/>-->
            <!--<glyph glyph-code="EB7E" smufl-name="arrowheadBlackLeft"/>-->
            <!--<glyph glyph-code="EB7F" smufl-name="arrowheadBlackUpLeft"/>-->
            <!--<glyph glyph-code="EB80" smufl-name="arrowheadWhiteUp"/>-->
            <!--<glyph glyph-code="EB81" smufl-name="arrowheadWhiteUpRight"/>-->
            <!--<glyph glyph-code="EB82" smufl-name="arrowheadWhiteRight"/>-->
            <!--<glyph glyph-code="EB83" smufl-name="arrowheadWhiteDownRight"/>-->
            <!--<glyph glyph-code="EB84" smufl-name="arrowheadWhiteDown"/>-->
            <!--<glyph glyph-code="EB85" smufl-name="arrowheadWhiteDownLeft"/>-->
            <!--<glyph glyph-code="EB86" smufl-name="arrowheadWhiteLeft"/>-->
            <!--<glyph glyph-code="EB87" smufl-name="arrowheadWhiteUpLeft"/>-->
            <!--<glyph glyph-code="EB88" smufl-name="arrowheadOpenUp"/>-->
            <!--<glyph glyph-code="EB89" smufl-name="arrowheadOpenUpRight"/>-->
            <!--<glyph glyph-code="EB8A" smufl-name="arrowheadOpenRight"/>-->
            <!--<glyph glyph-code="EB8B" smufl-name="arrowheadOpenDownRight"/>-->
            <!--<glyph glyph-code="EB8C" smufl-name="arrowheadOpenDown"/>-->
            <!--<glyph glyph-code="EB8D" smufl-name="arrowheadOpenDownLeft"/>-->
            <!--<glyph glyph-code="EB8E" smufl-name="arrowheadOpenLeft"/>-->
            <!--<glyph glyph-code="EB8F" smufl-name="arrowheadOpenUpLeft"/>-->
            <range_end>U+EB8F</range_end>
            <range_start>U+EB60</range_start>
        </arrowsAndArrowheads>
        <combiningStaffPositions>
            <description>Combining staff positions</description>
            <!--<glyph glyph-code="EB90" smufl-name="staffPosRaise1"/>-->
            <!--<glyph glyph-code="EB91" smufl-name="staffPosRaise2"/>-->
            <!--<glyph glyph-code="EB92" smufl-name="staffPosRaise3"/>-->
            <!--<glyph glyph-code="EB93" smufl-name="staffPosRaise4"/>-->
            <!--<glyph glyph-code="EB94" smufl-name="staffPosRaise5"/>-->
            <!--<glyph glyph-code="EB95" smufl-name="staffPosRaise6"/>-->
            <!--<glyph glyph-code="EB96" smufl-name="staffPosRaise7"/>-->
            <!--<glyph glyph-code="EB97" smufl-name="staffPosRaise8"/>-->
            <!--<glyph glyph-code="EB98" smufl-name="staffPosLower1"/>-->
            <!--<glyph glyph-code="EB99" smufl-name="staffPosLower2"/>-->
            <!--<glyph glyph-code="EB9A" smufl-name="staffPosLower3"/>-->
            <!--<glyph glyph-code="EB9B" smufl-name="staffPosLower4"/>-->
            <!--<glyph glyph-code="EB9C" smufl-name="staffPosLower5"/>-->
            <!--<glyph glyph-code="EB9D" smufl-name="staffPosLower6"/>-->
            <!--<glyph glyph-code="EB9E" smufl-name="staffPosLower7"/>-->
            <!--<glyph glyph-code="EB9F" smufl-name="staffPosLower8"/>-->
            <range_end>U+EB9F</range_end>
            <range_start>U+EB90</range_start>
        </combiningStaffPositions>
        <renaissanceLuteTablature>
            <description>Renaissance lute tablature</description>
            <!--<glyph glyph-code="EBA0" smufl-name="luteStaff6Lines"/>-->
            <!--<glyph glyph-code="EBA1" smufl-name="luteStaff6LinesWide"/>-->
            <!--<glyph glyph-code="EBA2" smufl-name="luteStaff6LinesNarrow"/>-->
            <!--<glyph glyph-code="EBA3" smufl-name="luteBarlineStartRepeat"/>-->
            <!--<glyph glyph-code="EBA4" smufl-name="luteBarlineEndRepeat"/>-->
            <!--<glyph glyph-code="EBA5" smufl-name="luteBarlineFinal"/>-->
            <!--<glyph glyph-code="EBA6" smufl-name="luteDurationDoubleWhole"/>-->
            <glyph glyph-code="EBA7" smufl-name="luteDurationWhole"/>
            <glyph glyph-code="EBA8" smufl-name="luteDurationHalf"/>
            <glyph glyph-code="EBA9" smufl-name="luteDurationQuarter"/>
            <glyph glyph-code="EBAA" smufl-name="luteDuration8th"/>
            <glyph glyph-code="EBAB" smufl-name="luteDuration16th"/>
            <glyph glyph-code="EBAC" smufl-name="luteDuration32nd"/>
            <!--<glyph glyph-code="EBAD" smufl-name="luteFingeringRHThumb"/>-->
            <!--<glyph glyph-code="EBAE" smufl-name="luteFingeringRHFirst"/>-->
            <!--<glyph glyph-code="EBAF" smufl-name="luteFingeringRHSecond"/>-->
            <!--<glyph glyph-code="EBB0" smufl-name="luteFingeringRHThird"/>-->
            <range_end>U+EBBF</range_end>
            <range_start>U+EBA0</range_start>
        </renaissanceLuteTablature>
        <frenchAndEnglishRenaissanceLuteTablature>
            <description>French and English Renaissance lute tablature</description>
            <!--<glyph glyph-code="EBC0" smufl-name="luteFrenchFretA"/>-->
            <!--<glyph glyph-code="EBC1" smufl-name="luteFrenchFretB"/>-->
            <!--<glyph glyph-code="EBC2" smufl-name="luteFrenchFretC"/>-->
            <!--<glyph glyph-code="EBC3" smufl-name="luteFrenchFretD"/>-->
            <!--<glyph glyph-code="EBC4" smufl-name="luteFrenchFretE"/>-->
            <!--<glyph glyph-code="EBC5" smufl-name="luteFrenchFretF"/>-->
            <!--<glyph glyph-code="EBC6" smufl-name="luteFrenchFretG"/>-->
            <!--<glyph glyph-code="EBC7" smufl-name="luteFrenchFretH"/>-->
            <!--<glyph glyph-code="EBC8" smufl-name="luteFrenchFretI"/>-->
            <!--<glyph glyph-code="EBC9" smufl-name="luteFrenchFretK"/>-->
            <!--<glyph glyph-code="EBCA" smufl-name="luteFrenchFretL"/>-->
            <!--<glyph glyph-code="EBCB" smufl-name="luteFrenchFretM"/>-->
            <!--<glyph glyph-code="EBCC" smufl-name="luteFrenchFretN"/>-->
            <!--<glyph glyph-code="EBCD" smufl-name="luteFrench7thCourse"/>-->
            <!--<glyph glyph-code="EBCE" smufl-name="luteFrench8thCourse"/>-->
            <!--<glyph glyph-code="EBCF" smufl-name="luteFrench9thCourse"/>-->
            <!--<glyph glyph-code="EBD0" smufl-name="luteFrench10thCourse"/>-->
            <!--<glyph glyph-code="EBD1" smufl-name="luteFrenchMordentUpper"/>-->
            <!--<glyph glyph-code="EBD2" smufl-name="luteFrenchMordentLower"/>-->
            <!--<glyph glyph-code="EBD3" smufl-name="luteFrenchMordentInverted"/>-->
            <!--<glyph glyph-code="EBD4" smufl-name="luteFrenchAppoggiaturaBelow"/>-->
            <!--<glyph glyph-code="EBD5" smufl-name="luteFrenchAppoggiaturaAbove"/>-->
            <range_end>U+EBDF</range_end>
            <range_start>U+EBC0</range_start>
        </frenchAndEnglishRenaissanceLuteTablature>
        <italianAndSpanishRenaissanceLuteTablature>
            <description>Italian and Spanish Renaissance lute tablature</description>
            <glyph glyph-code="EBE0" smufl-name="luteItalianFret0"/>
            <glyph glyph-code="EBE1" smufl-name="luteItalianFret1"/>
            <glyph glyph-code="EBE2" smufl-name="luteItalianFret2"/>
            <glyph glyph-code="EBE3" smufl-name="luteItalianFret3"/>
            <glyph glyph-code="EBE4" smufl-name="luteItalianFret4"/>
            <glyph glyph-code="EBE5" smufl-name="luteItalianFret5"/>
            <glyph glyph-code="EBE6" smufl-name="luteItalianFret6"/>
            <glyph glyph-code="EBE7" smufl-name="luteItalianFret7"/>
            <glyph glyph-code="EBE8" smufl-name="luteItalianFret8"/>
            <glyph glyph-code="EBE9" smufl-name="luteItalianFret9"/>
            <!--<glyph glyph-code="EBEA" smufl-name="luteItalianTempoFast"/>-->
            <!--<glyph glyph-code="EBEB" smufl-name="luteItalianTempoSomewhatFast"/>-->
            <!--<glyph glyph-code="EBEC" smufl-name="luteItalianTempoNeitherFastNorSlow"/>-->
            <!--<glyph glyph-code="EBED" smufl-name="luteItalianTempoSlow"/>-->
            <!--<glyph glyph-code="EBEE" smufl-name="luteItalianTempoVerySlow"/>-->
            <!--<glyph glyph-code="EBEF" smufl-name="luteItalianTimeTriple"/>-->
            <!--<glyph glyph-code="EBF0" smufl-name="luteItalianClefFFaUt"/>-->
            <!--<glyph glyph-code="EBF1" smufl-name="luteItalianClefCSolFaUt"/>-->
            <!--<glyph glyph-code="EBF2" smufl-name="luteItalianTremolo"/>-->
            <!--<glyph glyph-code="EBF3" smufl-name="luteItalianHoldNote"/>-->
            <!--<glyph glyph-code="EBF4" smufl-name="luteItalianHoldFinger"/>-->
            <!--<glyph glyph-code="EBF5" smufl-name="luteItalianReleaseFinger"/>-->
            <!--<glyph glyph-code="EBF6" smufl-name="luteItalianVibrato"/>-->
            <range_end>U+EBFF</range_end>
            <range_start>U+EBE0</range_start>
        </italianAndSpanishRenaissanceLuteTablature>
        <germanRenaissanceLuteTablature>
            <description>German Renaissance lute tablature</description>
            <!--<glyph glyph-code="EC00" smufl-name="luteGermanALower"/>-->
            <!--<glyph glyph-code="EC01" smufl-name="luteGermanBLower"/>-->
            <!--<glyph glyph-code="EC02" smufl-name="luteGermanCLower"/>-->
            <!--<glyph glyph-code="EC03" smufl-name="luteGermanDLower"/>-->
            <!--<glyph glyph-code="EC04" smufl-name="luteGermanELower"/>-->
            <!--<glyph glyph-code="EC05" smufl-name="luteGermanFLower"/>-->
            <!--<glyph glyph-code="EC06" smufl-name="luteGermanGLower"/>-->
            <!--<glyph glyph-code="EC07" smufl-name="luteGermanHLower"/>-->
            <!--<glyph glyph-code="EC08" smufl-name="luteGermanILower"/>-->
            <!--<glyph glyph-code="EC09" smufl-name="luteGermanKLower"/>-->
            <!--<glyph glyph-code="EC0A" smufl-name="luteGermanLLower"/>-->
            <!--<glyph glyph-code="EC0B" smufl-name="luteGermanMLower"/>-->
            <!--<glyph glyph-code="EC0C" smufl-name="luteGermanNLower"/>-->
            <!--<glyph glyph-code="EC0D" smufl-name="luteGermanOLower"/>-->
            <!--<glyph glyph-code="EC0E" smufl-name="luteGermanPLower"/>-->
            <!--<glyph glyph-code="EC0F" smufl-name="luteGermanQLower"/>-->
            <!--<glyph glyph-code="EC10" smufl-name="luteGermanRLower"/>-->
            <!--<glyph glyph-code="EC11" smufl-name="luteGermanSLower"/>-->
            <!--<glyph glyph-code="EC12" smufl-name="luteGermanTLower"/>-->
            <!--<glyph glyph-code="EC13" smufl-name="luteGermanVLower"/>-->
            <!--<glyph glyph-code="EC14" smufl-name="luteGermanXLower"/>-->
            <!--<glyph glyph-code="EC15" smufl-name="luteGermanYLower"/>-->
            <!--<glyph glyph-code="EC16" smufl-name="luteGermanZLower"/>-->
            <!--<glyph glyph-code="EC17" smufl-name="luteGermanAUpper"/>-->
            <!--<glyph glyph-code="EC18" smufl-name="luteGermanBUpper"/>-->
            <!--<glyph glyph-code="EC19" smufl-name="luteGermanCUpper"/>-->
            <!--<glyph glyph-code="EC1A" smufl-name="luteGermanDUpper"/>-->
            <!--<glyph glyph-code="EC1B" smufl-name="luteGermanEUpper"/>-->
            <!--<glyph glyph-code="EC1C" smufl-name="luteGermanFUpper"/>-->
            <!--<glyph glyph-code="EC1D" smufl-name="luteGermanGUpper"/>-->
            <!--<glyph glyph-code="EC1E" smufl-name="luteGermanHUpper"/>-->
            <!--<glyph glyph-code="EC1F" smufl-name="luteGermanIUpper"/>-->
            <!--<glyph glyph-code="EC20" smufl-name="luteGermanKUpper"/>-->
            <!--<glyph glyph-code="EC21" smufl-name="luteGermanLUpper"/>-->
            <!--<glyph glyph-code="EC22" smufl-name="luteGermanMUpper"/>-->
            <!--<glyph glyph-code="EC23" smufl-name="luteGermanNUpper"/>-->
            <range_end>U+EC2F</range_end>
            <range_start>U+EC00</range_start>
        </germanRenaissanceLuteTablature>
        <kievanSquareNotation>
            <description>Kievan square notation</description>
            <!--<glyph glyph-code="EC30" smufl-name="kievanCClef"/>-->
            <!--<glyph glyph-code="EC31" smufl-name="kievanEndingSymbol"/>-->
            <!--<glyph glyph-code="EC32" smufl-name="kievanNoteReciting"/>-->
            <!--<glyph glyph-code="EC33" smufl-name="kievanNoteWhole"/>-->
            <!--<glyph glyph-code="EC34" smufl-name="kievanNoteWholeFinal"/>-->
            <!--<glyph glyph-code="EC35" smufl-name="kievanNoteHalfStaffLine"/>-->
            <!--<glyph glyph-code="EC36" smufl-name="kievanNoteHalfStaffSpace"/>-->
            <!--<glyph glyph-code="EC37" smufl-name="kievanNoteQuarterStemUp"/>-->
            <!--<glyph glyph-code="EC38" smufl-name="kievanNoteQuarterStemDown"/>-->
            <!--<glyph glyph-code="EC39" smufl-name="kievanNote8thStemUp"/>-->
            <!--<glyph glyph-code="EC3A" smufl-name="kievanNote8thStemDown"/>-->
            <!--<glyph glyph-code="EC3B" smufl-name="kievanNoteBeam"/>-->
            <!--<glyph glyph-code="EC3C" smufl-name="kievanAugmentationDot"/>-->
            <!--<glyph glyph-code="EC3D" smufl-name="kievanAccidentalSharp"/>-->
            <!--<glyph glyph-code="EC3E" smufl-name="kievanAccidentalFlat"/>-->
            <range_end>U+EC3F</range_end>
            <range_start>U+EC30</range_start>
        </kievanSquareNotation>
        <kodályHandSigns>
            <description>Kodály hand signs</description>
            <!--<glyph glyph-code="EC40" smufl-name="kodalyHandDo"/>-->
            <!--<glyph glyph-code="EC41" smufl-name="kodalyHandRe"/>-->
            <!--<glyph glyph-code="EC42" smufl-name="kodalyHandMi"/>-->
            <!--<glyph glyph-code="EC43" smufl-name="kodalyHandFa"/>-->
            <!--<glyph glyph-code="EC44" smufl-name="kodalyHandSo"/>-->
            <!--<glyph glyph-code="EC45" smufl-name="kodalyHandLa"/>-->
            <!--<glyph glyph-code="EC46" smufl-name="kodalyHandTi"/>-->
            <range_end>U+EC4F</range_end>
            <range_start>U+EC40</range_start>
        </kodályHandSigns>
        <simplifiedMusicNotation>
            <description>Simplified Music Notation</description>
            <!--<glyph glyph-code="EC50" smufl-name="smnSharp"/>-->
            <!--<glyph glyph-code="EC51" smufl-name="smnSharpWhite"/>-->
            <!--<glyph glyph-code="EC52" smufl-name="smnFlat"/>-->
            <!--<glyph glyph-code="EC53" smufl-name="smnFlatWhite"/>-->
            <!--<glyph glyph-code="EC54" smufl-name="smnHistorySharp"/>-->
            <!--<glyph glyph-code="EC55" smufl-name="smnHistoryDoubleSharp"/>-->
            <!--<glyph glyph-code="EC56" smufl-name="smnHistoryFlat"/>-->
            <!--<glyph glyph-code="EC57" smufl-name="smnHistoryDoubleFlat"/>-->
            <!--<glyph glyph-code="EC58" smufl-name="smnNatural"/>-->
            <range_end>U+EC5F</range_end>
            <range_start>U+EC50</range_start>
        </simplifiedMusicNotation>
        <miscellaneousSymbols>
            <description>Miscellaneous symbols</description>
            <!--<glyph glyph-code="EC60" smufl-name="miscDoNotPhotocopy"/>-->
            <!--<glyph glyph-code="EC61" smufl-name="miscDoNotCopy"/>-->
            <!--<glyph glyph-code="EC62" smufl-name="miscEyeglasses"/>-->
            <!--<glyph glyph-code="EC63" smufl-name="metricModulationArrowLeft"/>-->
            <!--<glyph glyph-code="EC64" smufl-name="metricModulationArrowRight"/>-->
            <range_end>U+EC6F</range_end>
            <range_start>U+EC60</range_start>
        </miscellaneousSymbols>
    </xsl:variable>
</xsl:stylesheet>
