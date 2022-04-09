/**
  @file
  @author Stefan Frings
*/

#include "../global.h"
#include "qsqlerror.h"
#include "templatecontroller.h"
#include "templatecache.h"
#include "template.h"
#include <QSqlQuery>

TemplateController::TemplateController()
{}

void TemplateController::service(HttpRequest& request, HttpResponse& response)
{
    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    Template t=templateCache->getTemplate("base",request.getHeader("Accept-Language"));
    t.enableWarnings();

    QString main_news;
    QSqlQuery *query;
    query = new QSqlQuery(grdo_db);

    if( !query->exec("SELECT * FROM news ORDER BY dt_posted DESC LIMIT 9") )
    {
        t.setVariable("main", query->lastError().text());
    }
    else
    {
        main_news += "<div class='row row-cols-1 row-cols-sm-2 row-cols-md-3 g-3'>";
        while( query->next() )
        {
            main_news += "<div class='col'><div class='card shadow-sm'>";
            main_news += "<img src='" + query->value(5).toString() + "' alt='" + query->value(3).toString() + "'>";
            main_news += "<div class='card-body'><p class='card-text'>" + query->value(3).toString() + "</p><div class='d-flex justify-content-between align-items-center'><small class='text-muted'>9 mins</small></div></div>";
            main_news += "</div></div>";
        }
        main_news += "</div>";
        t.setVariable("main", main_news);
    }

    response.write(t.toUtf8(),true);
}
