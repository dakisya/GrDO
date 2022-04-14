<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Gr - Dean's Office</title>
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css">
</head>
<body>
    <nav class="py-2 bg-light border-bottom">
        <div class="container d-flex flex-wrap">
          <ul class="nav me-auto">
            <li class="nav-item"><a href="/main/news" class="nav-link link-dark px-2 active" aria-current="page">Новости</a></li>
            <li class="nav-item"><a href="/main/disciplines" class="nav-link link-dark px-2">Дисциплины</a></li>
            <li class="nav-item"><a href="/main/reference" class="nav-link link-dark px-2">Справки</a></li>
            <li class="nav-item"><a href="/main/do" class="nav-link link-dark px-2">Деканат</a></li>
            <li class="nav-item"><a href="/main/faq" class="nav-link link-dark px-2">FAQ</a></li>
          </ul>
          <ul class="nav">
            <li class="nav-item"><a href="/admin" class="nav-link link-dark px-2">Admin</a></li>
          </ul>
        </div>
     </nav>

    <header class="py-3 mb-4 border-bottom">
        <div class="container d-flex flex-wrap justify-content-center">
          <a href="/" class="d-flex align-items-center mb-3 mb-lg-0 me-lg-auto text-dark text-decoration-none">
            <svg class="bi me-2" width="40" height="32"><use xlink:href="#bootstrap"></use></svg>
            <span class="fs-4">Наименование ВУЗа</span>
          </a>
        </div>
    </header>


    <div class="container py-3">
	<main>
            <h1 class="mt-5">{subject}</h1>
            <p class="lead">{body}</p>
            <p><a href="/main/news">К списку новостей</a></p>
	</main>
    </div>


    <div class="container">
      <footer class="py-5">
        <div class="d-flex justify-content-between py-4 my-4 border-top">
          <p>© 2022 @dakisya. All rights reserved.</p>
        </div>
      </footer>
    </div>
</body>
</html>
