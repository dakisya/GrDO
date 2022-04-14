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


QString main_window()
{
    QString main_news;
    QSqlQuery *query;
    query = new QSqlQuery(grdo_db);

    if( !query->exec("SELECT * FROM news ORDER BY dt_posted DESC LIMIT 9") )
    {
        main_news = query->lastError().text();
    }
    else
    {
        main_news = "<div class='row row-cols-1 row-cols-sm-2 row-cols-md-3 g-3'>";
        while( query->next() )
        {
            main_news += "<div class='col'><div class='card shadow-sm'>";
            main_news += "<img src='/" + query->value(5).toString() + "' alt='" + query->value(3).toString() + "'>";
            main_news += "<div class='card-body'><p class='card-text'>" + query->value(3).toString() + "</p><div class='d-flex justify-content-between align-items-center'><small class='text-muted'>9 mins</small></div></div>";
            main_news += "</div></div>";
        }
        main_news += "</div>";
    }

    return main_news;
}


QString get_news_list()
{
    QString news_list;
    QSqlQuery *query;
    query = new QSqlQuery(grdo_db);

    if( !query->exec("SELECT * FROM news ORDER BY dt_posted") )
    {
        news_list = query->lastError().text();
    }
    else
    {
        news_list += "<div class='my-3 p-3 bg-body rounded shadow-sm'><h6 class='border-bottom pb-2 mb-0'>Новости</h6>";
        while( query->next() )
        {
            news_list += "<div class='d-flex text-muted pt-3'>";
            news_list += "<img class='bd-placeholder-img flex-shrink-0 me-2 rounded' width='32' height='32' src='/" + query->value(5).toString() + "' alt='" + query->value(3).toString() + "'>";
            news_list += "<p class='pb-3 mb-0 small lh-sm'><strong><a href='/main/news/" + query->value(0).toString() + "' class='d-block text-muted text-gray-dark'>" + query->value(3).toString() + "</a></strong>" + query->value(4).toString() + "</p>";
            news_list += "</div>";
            news_list += "<div class='nav justify-content-left border-bottom pb-1 mb-3'>";
            news_list += "<a href = '/main/news/edit/" + query->value(0).toString() + "' class='nav-link p-0 text-muted small'>Изменить</a>&nbsp|&nbsp<a href = '/main/news/del/" + query->value(0).toString() + "' class='nav-link p-0 text-muted small'>Удалить</a>";
            //news_list += "<a href = '/main/news/del/" + query->value(0).toString() + "'>Удалить</a>";
            news_list += "</div>";
        }
        news_list += "<small class='d-block text-end mt-3'><a href='#'>All updates</a></small></div>";
    }

    return news_list;
}


QString get_disciplines_list()
{
    QString disciplines_list;
    disciplines_list += "Раздел с описание дисциплин";
    return disciplines_list;
}


QString get_faq_list()
{
    QString faq_list;
    faq_list += "Раздел с вопросами и ответами";
    return faq_list;
}


QString get_reference_list()
{
    QString reference_list;
    reference_list += "Раздел со справками";
    return reference_list;
}

QString get_do_info()
{
    QString do_info;
    do_info += "Раздел для деканата";
    return do_info;
}



void TemplateController::service(HttpRequest& request, HttpResponse& response)
{
    QString tpl = "base";
    QString main_content;

    QByteArray path=request.getPath();
    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    if (path == "/main/news") {
        main_content = get_news_list();
    }
    else if (path == "/main/disciplines") {
         main_content = get_disciplines_list();
    }
    else if (path == "/main/faq"){
         main_content = get_faq_list();
    }
    else if (path == "/main/reference"){
        main_content = get_reference_list();
    }
    else if (path == "/main/do"){
         main_content = get_do_info();
    }
    else if (path == "/" || path == "") {
        main_content = main_window();
        //main_content = "<h1>Что-то пошло не так как задумывалось. Сейчас поправим!</h1>";
    }

    if (main_content != "") {
        Template main_tpl = templateCache->getTemplate(tpl,request.getHeader("Accept-Language"));
        main_tpl.enableWarnings();
        main_tpl.setVariable("main", main_content);
        response.write(main_tpl.toUtf8(),true);
    }
    else if (path.startsWith("/main/news/")){
        QString news_idx = path;
        QString news_action = "view";
        news_action = news_idx.section("/", -2, -2);
        news_idx = news_idx.section("/", -1);

        QString news_list;
        QSqlQuery *query;
        query = new QSqlQuery(grdo_db);

        qInfo() << news_action << news_idx;

        if (news_action == "news"){
            Template news_tpl = templateCache->getTemplate("news",request.getHeader("Accept-Language"));
            news_tpl.enableWarnings();

            if( !query->exec("SELECT * FROM news WHERE id = " + news_idx) )
            {
                news_tpl.setVariable("subject", "Ошибка выполнения запроса!");
                news_tpl.setVariable("body", query->lastError().text());
            }
            else
            {
                while( query->next() ){
                    news_tpl.setVariable("subject", query->value(3).toString());
                    news_tpl.setVariable("body", query->value(4).toString());
                }
            }
            response.write(news_tpl.toUtf8(),true);
        }
        else if (news_action == "del"){
            if( !query->exec("DELETE FROM news WHERE id = " + news_idx) )
            {
                qDebug() << query->lastError().text();
                response.write("</script><script>setTimeout(function(){window.location.href = '/';}, 1 * 1000);");
            }
            else
            {
                grdo_db.commit();
                response.write("<script>setTimeout(function(){window.location.href = '/main/news';}, 1 * 1000);</script>");
            }
        }
        else if (news_action == "edit"){
            Template admin_tpl = templateCache->getTemplate("admin",request.getHeader("Accept-Language"));
            admin_tpl.enableWarnings();
            admin_tpl.setVariable("title", "Изменение новостной записи");
            admin_tpl.setVariable("action", "edit");

            if( !query->exec("SELECT * FROM news WHERE id = " + news_idx) )
            {
                admin_tpl.setVariable("subject", "Ошибка выполнения запроса!");
                admin_tpl.setVariable("body", query->lastError().text());
            }
            else
            {
                while( query->next() ){
                    admin_tpl.setVariable("subject", query->value(3).toString());
                    admin_tpl.setVariable("body", query->value(4).toString());
                }
            }
            response.write(admin_tpl.toUtf8(),true);
            //UPDATE news set subject='Update test', body = 'Update body test' Where id = 8
        }
    }
    else if (path.startsWith("/admin")){
        QString news_idx = path;
        news_idx = news_idx.section("/", -1);

        QSqlQuery *query;
        query = new QSqlQuery(grdo_db);
        QString sq;

        response.setHeader("Content-Type", "text/html; charset=UTF-8");

        if (request.getParameter("action")=="add")
        {

            sq = "INSERT INTO news(dt_created, dt_posted, subject, body, img_url) VALUES (CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, '" + QString(request.getParameter("subject")) + "', '" +  QString(request.getParameter("body")) + "', 'pic1.jpg');";
            query->exec(sq);
            grdo_db.commit();

            response.write("<html><body>");
            response.write("sql done");
            response.write("<script>setTimeout(function(){window.location.href = '/admin';}, 3 * 1000);</script>");
            response.write("</body></html>",true);
        }
        else if (request.getParameter("action")=="edit") {
            sq = "UPDATE news set subject='" + QString(request.getParameter("subject")) + "', body = '" +  QString(request.getParameter("body")) + "' Where id = " + news_idx;
            query->exec(sq);
            grdo_db.commit();

            response.write("<html><body>");
            response.write("sql done");
            response.write("<script>setTimeout(function(){window.location.href = '/admin';}, 3 * 1000);</script>");
            response.write("</body></html>",true);
        }
        else
        {
            Template admin_tpl = templateCache->getTemplate("admin",request.getHeader("Accept-Language"));
            admin_tpl.enableWarnings();
            admin_tpl.setVariable("title", "Новая новостная запись");
            admin_tpl.setVariable("action", "add");
            admin_tpl.setVariable("subject", "");
            admin_tpl.setVariable("body", "");
            response.write(admin_tpl.toUtf8(),true);
        }
    }
}
