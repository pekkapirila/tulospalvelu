<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:foo="http://www.foo.org/" xmlns:bar="http://www.bar.org">
<xsl:template match="/">
    <HTML>
	<head>
		<style>
		body {font-family:Verdana,Arial,Sans-serif;font-size:medium;}
		h2 {font-size:150%;}
		h3 {font-size:120%}
		table, td, th {border:1px solid lightgrey;}
		table {border-collapse: collapse;}
		td {border:0; padding:10px 5px 10px 5px;}
		th {border:0; font-weight:bold; padding:10px 5px 10px 5px;}
		</style>
	</head>
      <BODY>
        <xsl:for-each select="HylRaportti">
		<h2>Hylkäysesitys</h2>
		<h3>Disqualification proposal</h3>
		<p> </p><p> </p>
		<table width="100%">
			<colgroup>
			<col width="50%"/>
			<col width="50%"/>
			</colgroup>
			<tr><td>Tapaus avattu / Case opened</td><td><xsl:value-of select="AvausAika"/></td></tr>
			<tr><td>Viimeisin tallennus / Latest processing</td><td><xsl:value-of select="TallAika"/></td></tr>
			<tr><td>Sarja / Class</td><td><xsl:value-of select="Sarja"/></td></tr>
			<tr><td>Joukkue / Team</td>
			<td><xsl:value-of select="KilpNo"/>: <xsl:value-of select="Joukkue"/></td>
			</tr>
			<tr><td>Osuus / Leg</td>
			<td><xsl:value-of select="Osuus"/>: <xsl:value-of select="Nimi"/></td>
			</tr>
			<tr><td>Emit-koodi / Emit code</td><td><xsl:value-of select="Emit"/></td></tr>
			<tr><td>Rata / Course</td><td><xsl:value-of select="Rata"/></td></tr>
			<tr><td>Ongelma / Problem</td><td><xsl:value-of select="Luonne"/></td></tr>
			<tr><td>Piikinjäljet / Control card</td><td><xsl:value-of select="Piikit"/></td></tr>
			<tr><td>Lisätietoa / Additional notes</td><td><xsl:value-of select="Selitys"/></td></tr>
			<xsl:for-each select="Muistio">
				<tr><td style='padding:0px 5px 0px 5px'></td><td style='padding:0px 5px 0px 5px'><xsl:value-of select="Rivi"/></td></tr>
			</xsl:for-each>
			<tr><td>Toimitsijan päätös / Official's decision</td><td><xsl:value-of select="Paatos_1"/></td></tr>
			<tr><td>Toimitsija / Official</td><td><xsl:value-of select="Toimitsija"/></td></tr>
			<tr><td>Kilpailijan kanta / Competitor's view</td><td><xsl:value-of select="Myonto"/></td></tr>
			<tr><td>Lopullinen päätös / Final decision</td><td><xsl:value-of select="Paatos_2"/></td></tr>
          </table>
       </xsl:for-each>
      </BODY>
    </HTML>
  </xsl:template>
</xsl:stylesheet>