TRUNCATE TABLE order_discounts,
discounts,
interested_packages,
order_packages,
orders,
travel_packages,
hotels,
countries
RESTART IDENTITY CASCADE;

INSERT INTO
    countries (
        country_name,
        climate,
        recreation_features
    )
VALUES (
        'Россия',
        'Умеренный, на юге субтропический',
        'Городской, экскурсионный и пляжный отдых на юге страны'
    ),
    (
        'Турция',
        'Средиземноморский и субтропический',
        'Пляжный отдых, отели all inclusive, экскурсии и семейный туризм'
    ),
    (
        'Египет',
        'Жаркий пустынный',
        'Пляжный отдых, Красное море, дайвинг и экскурсии'
    ),
    (
        'ОАЭ',
        'Жаркий пустынный',
        'Городской отдых, пляжи, шопинг и современные достопримечательности'
    ),
    (
        'Таиланд',
        'Тропический муссонный',
        'Пляжный отдых, острова, экскурсии и оздоровительный туризм'
    ),
    (
        'Кипр',
        'Средиземноморский',
        'Пляжный отдых, спокойные курорты и семейный туризм'
    ),
    (
        'Греция',
        'Средиземноморский',
        'Пляжный отдых, острова и античные достопримечательности'
    ),
    (
        'Италия',
        'Средиземноморский',
        'Экскурсионный туризм, культура, гастрономия и морские курорты'
    ),
    (
        'Испания',
        'Средиземноморский',
        'Пляжный отдых, экскурсии, островные и городские направления'
    ),
    (
        'Франция',
        'Умеренный и средиземноморский',
        'Экскурсионный туризм, культурный отдых и Лазурный берег'
    ),
    (
        'Черногория',
        'Средиземноморский',
        'Пляжный отдых, горные маршруты и спокойные курорты'
    ),
    (
        'Мальдивы',
        'Тропический',
        'Пляжный отдых, островные курорты, дайвинг и романтические туры'
    ),
    (
        'Индонезия',
        'Экваториальный и тропический',
        'Пляжный отдых, острова, природа и культурные экскурсии'
    ),
    (
        'Вьетнам',
        'Тропический и субтропический',
        'Пляжный отдых, экскурсии, городские и природные маршруты'
    ),
    (
        'Грузия',
        'Субтропический и умеренный',
        'Экскурсионный отдых, горы, гастрономия и морское побережье'
    );

INSERT INTO
    hotels (
        id_country,
        hotel_name,
        hotel_class,
        hotel_address
    )
VALUES (
        1,
        'Radisson Collection Hotel Moscow',
        '5 звезд',
        'Москва, Кутузовский проспект, 2/1'
    ),
    (
        1,
        'Hotel Metropol Moscow',
        '5 звезд',
        'Москва, Театральный проезд, 2'
    ),
    (
        1,
        'Cosmos Moscow VDNH Hotel',
        '3 звезды',
        'Москва, проспект Мира, 150'
    ),
    (
        1,
        'Отель Санкт-Петербург',
        '4 звезды',
        'Санкт-Петербург, Пироговская набережная, 5/2'
    ),
    (
        1,
        'Swissotel Resort Sochi Kamelia',
        '5 звезд',
        'Сочи, Курортный проспект, 89'
    ),
    (
        2,
        'Akra Antalya',
        '5 звезд',
        'Анталья, Lara Yolu'
    ),
    (
        2,
        'Rixos Premium Belek',
        '5 звезд',
        'Белек, Ileribasi Mevkii'
    ),
    (
        2,
        'Seven Seas Hotel Life',
        '5 звезд',
        'Кемер, Goynuk'
    ),
    (
        3,
        'Steigenberger Aqua Magic Red Sea',
        '5 звезд',
        'Хургада, Yusif Afifi Road'
    ),
    (
        3,
        'Jaz Aquamarine Resort',
        '5 звезд',
        'Хургада, South Sahl Hashish Road'
    ),
    (
        3,
        'Sultan Gardens Resort',
        '5 звезд',
        'Шарм-эш-Шейх, Sharks Bay'
    ),
    (
        4,
        'Rove Downtown Dubai',
        '3 звезды',
        'Дубай, 312 Al Mustaqbal Street'
    ),
    (
        4,
        'Atlantis The Palm',
        '5 звезд',
        'Дубай, Crescent Road, The Palm Jumeirah'
    ),
    (
        4,
        'Jumeirah Beach Hotel',
        '5 звезд',
        'Дубай, Jumeirah Street'
    ),
    (
        5,
        'Novotel Phuket Resort',
        '4 звезды',
        'Пхукет, Patong Beach'
    ),
    (
        5,
        'Centara Grand Beach Resort Phuket',
        '5 звезд',
        'Пхукет, Karon Beach'
    ),
    (
        5,
        'Amari Pattaya',
        '5 звезд',
        'Паттайя, Beach Road'
    ),
    (
        6,
        'Nissi Beach Resort',
        '4 звезды',
        'Айя-Напа, Nissi Avenue'
    ),
    (
        6,
        'Mediterranean Beach Hotel',
        '4 звезды',
        'Лимассол, Amathus Avenue'
    ),
    (
        7,
        'Electra Palace Athens',
        '5 звезд',
        'Афины, Navarchou Nikodimou Street'
    ),
    (
        7,
        'Mitsis Grand Hotel',
        '5 звезд',
        'Родос, Akti Miaouli'
    ),
    (
        7,
        'Creta Maris Resort',
        '5 звезд',
        'Крит, Херсониссос'
    ),
    (
        8,
        'Hotel Artemide',
        '4 звезды',
        'Рим, Via Nazionale'
    ),
    (
        8,
        'Starhotels Michelangelo Florence',
        '4 звезды',
        'Флоренция, Viale Fratelli Rosselli'
    ),
    (
        8,
        'Hotel Principe Venice',
        '4 звезды',
        'Венеция, Lista di Spagna'
    ),
    (
        9,
        'Hotel Regina Barcelona',
        '4 звезды',
        'Барселона, Carrer de Bergara'
    ),
    (
        9,
        'Melia Alicante',
        '4 звезды',
        'Аликанте, Plaza del Puerto'
    ),
    (
        9,
        'Iberostar Selection Playa de Palma',
        '5 звезд',
        'Майорка, Playa de Palma'
    ),
    (
        10,
        'Hotel Eiffel Blomet',
        '4 звезды',
        'Париж, Rue Blomet'
    ),
    (
        10,
        'Novotel Paris Centre Tour Eiffel',
        '4 звезды',
        'Париж, Quai de Grenelle'
    ),
    (
        10,
        'Hotel Nice Riviera',
        '4 звезды',
        'Ницца, Rue Pastorelli'
    ),
    (
        11,
        'Splendid Conference & Spa Resort',
        '5 звезд',
        'Будва, Becici'
    ),
    (
        11,
        'Hotel Montenegro Beach Resort',
        '4 звезды',
        'Бечичи, Budva Riviera'
    ),
    (
        12,
        'Sun Siyam Olhuveli',
        '5 звезд',
        'Мальдивы, South Male Atoll'
    ),
    (
        12,
        'Paradise Island Resort & Spa',
        '5 звезд',
        'Мальдивы, Lankanfinolhu'
    ),
    (
        13,
        'Grand Hyatt Bali',
        '5 звезд',
        'Бали, Nusa Dua'
    ),
    (
        13,
        'Bali Dynasty Resort',
        '5 звезд',
        'Бали, Kuta'
    ),
    (
        14,
        'Vinpearl Resort Nha Trang',
        '5 звезд',
        'Нячанг, Hon Tre Island'
    ),
    (
        14,
        'Muong Thanh Luxury Da Nang',
        '5 звезд',
        'Дананг, Vo Nguyen Giap Street'
    ),
    (
        15,
        'Rooms Hotel Tbilisi',
        '4 звезды',
        'Тбилиси, Kostava Street'
    ),
    (
        15,
        'Radisson Blu Hotel Batumi',
        '5 звезд',
        'Батуми, Ninoshvili Street'
    );


WITH hotel_base AS (
    SELECT
        h.id_hotel,
        h.hotel_class,
        h.id_country,
        row_number() OVER (ORDER BY h.id_hotel) AS rn,
        CASE c.country_name
            WHEN 'Россия' THEN 36000
            WHEN 'Турция' THEN 78000
            WHEN 'Египет' THEN 72000
            WHEN 'ОАЭ' THEN 105000
            WHEN 'Таиланд' THEN 98000
            WHEN 'Кипр' THEN 82000
            WHEN 'Греция' THEN 95000
            WHEN 'Италия' THEN 108000
            WHEN 'Испания' THEN 98000
            WHEN 'Франция' THEN 112000
            WHEN 'Черногория' THEN 76000
            WHEN 'Мальдивы' THEN 240000
            WHEN 'Индонезия' THEN 128000
            WHEN 'Вьетнам' THEN 92000
            WHEN 'Грузия' THEN 52000
            ELSE 70000
        END AS country_base,
        CASE
            WHEN h.hotel_class LIKE '5%' THEN 5
            WHEN h.hotel_class LIKE '4%' THEN 4
            WHEN h.hotel_class LIKE '3%' THEN 3
            ELSE 3
        END AS class_value
    FROM hotels h
    JOIN countries c ON c.id_country = h.id_country
),
package_rows AS (
    SELECT
        id_hotel,
        7 AS duration_days,
        (country_base + class_value * 7000 + (rn % 4) * 3500)::NUMERIC(10, 2) AS base_price,
        CASE
            WHEN class_value >= 5 THEN 'Проживание 7 дней, завтраки включены, трансфер, медицинская страховка'
            WHEN class_value = 4 THEN 'Проживание 7 дней, завтраки включены, групповая экскурсия'
            ELSE 'Проживание 7 дней, базовый тариф, медицинская страховка'
        END AS conditions
    FROM hotel_base

    UNION ALL

    SELECT
        id_hotel,
        14 AS duration_days,
        ROUND((country_base + class_value * 7000 + (rn % 4) * 3500) * 1.85, 2)::NUMERIC(10, 2) AS base_price,
        CASE
            WHEN class_value >= 5 THEN 'Проживание 14 дней, завтраки включены, трансфер, экскурсионная программа'
            WHEN class_value = 4 THEN 'Проживание 14 дней, завтраки включены, трансфер'
            ELSE 'Проживание 14 дней, базовый тариф, медицинская страховка'
        END AS conditions
    FROM hotel_base

    UNION ALL

    SELECT
        id_hotel,
        28 AS duration_days,
        ROUND((country_base + class_value * 7000 + (rn % 4) * 3500) * 3.40, 2)::NUMERIC(10, 2) AS base_price,
        CASE
            WHEN class_value >= 5 THEN 'Проживание 28 дней, завтраки включены, индивидуальный трансфер, расширенная страховка'
            WHEN class_value = 4 THEN 'Проживание 28 дней, завтраки включены, трансфер, расширенная страховка'
            ELSE 'Проживание 28 дней, базовый тариф, расширенная страховка'
        END AS conditions
    FROM hotel_base
    WHERE rn <= 20
)
INSERT INTO travel_packages (id_hotel, duration_days, base_price, conditions)
SELECT
    id_hotel,
    duration_days,
    base_price,
    conditions
FROM package_rows
ORDER BY id_hotel, duration_days;

INSERT INTO
    discounts (
        discount_name,
        discount_condition,
        discount_percent
    )
VALUES (
        'Скидка за количество путевок',
        'Предоставляется при покупке более двух путевок в одном заказе',
        10.00
    ),
    (
        'Скидка за сумму заказа',
        'Предоставляется при сумме заказа от 100000 рублей',
        5.00
    ),
    (
        'Скидка за одну страну',
        'Предоставляется, если все путевки в заказе относятся к одной стране',
        2.00
    );