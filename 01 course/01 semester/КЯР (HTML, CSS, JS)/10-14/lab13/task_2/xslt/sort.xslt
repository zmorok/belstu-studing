<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="/">
    <html>
      <head>
        <style type="text/css">
          table {
            border-collapse: collapse;
            width: 80%;
            background-color: #333;
          }
          th, td {
            border: 1px solid #dddddd;
            text-align: left;
            padding: 8px;
            color: white;
          }
          th {
            background-color: #121212;
          }
          .low-grade {
            background-color: red;
          }
          .high-grade {
            background-color: green;
          }
        </style>
      </head>
      <body>
        <h2>Аттестация студентов</h2>
        <table>
          <tr>
            <th>Имя</th>
            <th>Оценка</th>
          </tr>
          <xsl:for-each select="students/student">
            <xsl:sort order="ascending" select="name"/>
            <tr>
              <td><xsl:value-of select="name"/></td>

              <xsl:choose>
                <xsl:when test="grade &lt; 4">
                  <td bgcolor="red"><xsl:value-of select="grade"/></td>
                </xsl:when>
                <xsl:when test="grade &gt; 8">
                  <td bgcolor="green"><xsl:value-of select="grade"/></td>
                </xsl:when>
                <xsl:otherwise>
                  <td><xsl:value-of select="grade"/></td>
                </xsl:otherwise>
              </xsl:choose>
            </tr>
          </xsl:for-each>
        </table>
      </body>
    </html>
  </xsl:template>

</xsl:stylesheet>
