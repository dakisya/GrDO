/**
  @file
  @author Stefan Frings
*/

#include "formcontroller.h"
#include "src/global.h"
#include <QSqlQuery>
#include <QtSql>

FormController::FormController()
{}

void FormController::service(HttpRequest& request, HttpResponse& response)
{

    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    if (request.getParameter("action")=="show")
    {
        QSqlQuery *query;
        query = new QSqlQuery(grdo_db);
        QString sq;
        sq = "INSERT INTO news(dt_created, dt_posted, subject, body, img_url) VALUES (CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, '" + QString(request.getParameter("subject")) + "', '" +  QString(request.getParameter("body")) + "', 'pic1.jpg');";
        query->exec(sq);
        grdo_db.commit();

        response.write("<html><body>");
        response.write("sql done");
        response.write("<script>setTimeout(function(){window.location.href = '/';}, 3 * 1000);</script>");
        response.write("</body></html>",true);
    }
    else
    {
        response.write("<html><body>");
        response.write("<form method=\"post\">");
        response.write("  <input type=\"hidden\" name=\"action\" value=\"show\">");
        response.write("  Тема: <input type=\"text\" name=\"subject\"><br>");
        response.write("  Описание: <textarea rows=\"4\" cols=\"50\"></textarea><br>");
        response.write("  <input type=\"submit\">");
        response.write("</form>");
        response.write("</body></html>",true);
    }
}

