<?php
function d2daddon_statistics_create_users() {
  $n_user = variable_get('d2daddon_statistics_user_count', D2DADDON_STATISTICS_USER_COUNT);
  $schema_name = 'd2daddon_statistics_user';
  db_delete($schema_name)
    ->execute();
  for ($i = 0; $i < $n_user; $i++) {
    $id = db_insert($schema_name)
      ->fields(array(
        'name' => 'user' . $i,
        'posts' => rand(0, 40) + rand(0, 40),
        'votes' => rand(0, 10) + rand(0, 20),
      ))
      ->execute();
  }
}
function d2daddon_statistics_add_papers() {
  $schema_name = 'd2daddon_statistics_papers';
  db_delete($schema_name)
    ->execute();
  $papers = array(
    array(
      'author' => 'D. Helbing and W. Yu',
      'title' => 'The outbreak of cooperation among success-driven individuals under noisy conditions'
    ),
    array(
      'author' => 'D. Helbing, Y. Wenjian and H. Rauhut',
      'title' => 'Self-organization and emergence in social systems: Modeling the coevolution of social environments and cooperative behavior'
    ),
    array(
      'author' => 'D. Helbing',
      'title' => 'Managing complexity in socio-economic systems'
    ),
    array(
      'author' => 'J.R.G. Dyer, A. Johansson, D. Helbing, I.D. Couzin and J. Krause',
      'title' => 'Leadership, consensus decision making and collective behaviour in humans'
    ),
    array(
      'author' => 'D. Helbing',
      'title' => 'Pattern formation, social forces, and diffusion instability in games with success-driven motion'
    ),
    array(
      'author' => 'D. Helbing and A. Mazloumian',
      'title' => 'Operation regimes and slower-is-faster effect in the control of traffic intersections'
    )
  );
  foreach ($papers as $paper) {
    $id = db_insert($schema_name)
      ->fields(array(
            'author' => $paper['author'],
            'title' => $paper['title'],
            'rating' => rand(0, 100) * 0.01 + 4,
            'votes' => rand(10, 30),
            ))
      ->execute();
  }
}
function d2daddon_statistics_add_news() {
  $schema_name = 'd2daddon_statistics_news';
  db_delete($schema_name)
    ->execute();
  $type = variable_get('d2daddon_statistics_news_set', D2DADDON_STATISTICS_NEWS_SET);
  $author = variable_get('d2daddon_statistics_author', D2DADDON_STATISTICS_AUTHOR);
  $news = array();
  $news['a'] =
    array(
      array('title' => 'News mining might have predicted Arab Spring', 'time' => 1315915200),
      array('title' => 'When We\'re Cowed by the Crowd', 'time' => 1306584000),
      array('title' => 'A More Human Virtual Crowd.  A new vision-based approach could help prevent crowd disasters.', 'time' => 1303300800),
      array('title' => 'Earth project aims to \'simulate everything\'', 'time' => 1293537600),
      array('title' => 'Towards a socio-economic knowledge accelerator', 'time' => 1249473600),
    );

  $news['b'] =
    array(
      array('title' => 'What Causes Deadly \'Crowd-Quakes\'?', 'time' => 1303473600),
      array('title' => 'Wisdom about Crowds.  To model the behaviour of humans en masse, treat them as people, not molecules', 'time' => 1303300800),
      array('title' => 'Current Driving Conditions Michigan', 'time' => 1276689600),
      array('title' => 'about the Workshop on "Coping with Crises in Complex Socio-Economic Systems"', 'time' => 1248523200),
    );
  $news['c'] =
    array(
      array('title' => 'Predicting crowd behaviour', 'time' => 1303387200),
      array('title' => 'How crowds move: because people want to be free', 'time' => 1303214400),
      array('title' => 'Who should pay for the police?', 'time' => 1276516800),
      array('title' => 'CCSS Conference pictures and videos online', 'time' => 1248523200),
    );
  $news['d'] =
    array(
      array('title' => 'The strange but extremely valuable science of how pedestrians behave', 'time' => 1324123200),
      array('title' => 'Computational models to predict disasters', 'time' => 1308830400),
      array('title' => 'QScience 2012', 'time' => 1325376000),
    );
  foreach ($news[$type] as $newsrec) {
    $id = db_insert($schema_name)
      ->fields(array(
        'author' => $author,
        'title' => $newsrec['title'],
        'rating' => rand(0, 100) * 0.01 + 4,
        'votes' => rand(10, 30),
        'created' => $newsrec['time'],
      ))
      ->execute();
  }
}
