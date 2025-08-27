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
      </style>
    </head>
    <body>
      <h2>Каталог книг</h2>
      <table>
        <tr>
          <th>Название</th>
          <th>Автор</th>
          <th>Цена</th>
        </tr>
          <xsl:for-each select="catalog/book">
            <xsl:sort select="name"/>
            <tr>
              <td><xsl:value-of select="name"/></td>
              <td><xsl:value-of select="author"/></td>
              <td><xsl:value-of select="price"/></td>
            </tr>
          </xsl:for-each>
      </table>
    </body>
  </html>
</xsl:template>

</xsl:stylesheet>
