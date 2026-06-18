DROP TABLE IF EXISTS order_discounts CASCADE;

DROP TABLE IF EXISTS order_packages CASCADE;

DROP TABLE IF EXISTS interested_packages CASCADE;

DROP TABLE IF EXISTS discounts CASCADE;

DROP TABLE IF EXISTS orders CASCADE;

DROP TABLE IF EXISTS travel_packages CASCADE;

DROP TABLE IF EXISTS hotels CASCADE;

DROP TABLE IF EXISTS countries CASCADE;

DROP TABLE IF EXISTS clients CASCADE;

DROP TABLE IF EXISTS users CASCADE;

CREATE TABLE users (
    id_user INTEGER GENERATED ALWAYS AS IDENTITY,
    login VARCHAR(50) NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    role VARCHAR(20) NOT NULL,
    CONSTRAINT pk_users PRIMARY KEY (id_user),
    CONSTRAINT uq_users_login UNIQUE (login),
    CONSTRAINT chk_users_login_length CHECK (char_length(login) >= 3),
    CONSTRAINT chk_users_password_hash_not_empty CHECK (
        char_length(password_hash) > 0
    ),
    CONSTRAINT chk_users_role CHECK (role IN ('admin', 'client'))
);


CREATE TABLE clients (
    id_client INTEGER GENERATED ALWAYS AS IDENTITY,
    id_user INTEGER,
    last_name VARCHAR(50) NOT NULL,
    first_name VARCHAR(50) NOT NULL,
    patronymic VARCHAR(50),
    address TEXT NOT NULL,
    phone VARCHAR(20) NOT NULL,

    CONSTRAINT pk_clients PRIMARY KEY (id_client),

-- Связь Клиент 1:1 Пользователь.
-- UNIQUE нужен, чтобы один пользователь не был связан
-- с несколькими клиентами.


CONSTRAINT uq_clients_id_user UNIQUE (id_user),

    CONSTRAINT chk_clients_last_name_not_empty
        CHECK (char_length(trim(last_name)) > 0),

    CONSTRAINT chk_clients_first_name_not_empty
        CHECK (char_length(trim(first_name)) > 0),

    CONSTRAINT chk_clients_address_not_empty
        CHECK (char_length(trim(address)) > 0),

    CONSTRAINT chk_clients_phone_not_empty
        CHECK (char_length(trim(phone)) >= 5),

    CONSTRAINT fk_clients_users
        FOREIGN KEY (id_user)
        REFERENCES users (id_user)
        ON UPDATE CASCADE
        ON DELETE SET NULL
);


CREATE TABLE countries (
    id_country INTEGER GENERATED ALWAYS AS IDENTITY,
    country_name VARCHAR(100) NOT NULL,
    climate TEXT NOT NULL,
    recreation_features TEXT NOT NULL,

    CONSTRAINT pk_countries PRIMARY KEY (id_country),

-- Альтернативный ключ из реляционной модели.


CONSTRAINT uq_countries_country_name UNIQUE (country_name),

    CONSTRAINT chk_countries_country_name_not_empty
        CHECK (char_length(trim(country_name)) > 0),

    CONSTRAINT chk_countries_climate_not_empty
        CHECK (char_length(trim(climate)) > 0),

    CONSTRAINT chk_countries_recreation_features_not_empty
        CHECK (char_length(trim(recreation_features)) > 0)
);


CREATE TABLE hotels (
    id_hotel INTEGER GENERATED ALWAYS AS IDENTITY,
    id_country INTEGER NOT NULL,
    hotel_name VARCHAR(100) NOT NULL,
    hotel_class INTEGER NOT NULL,
    hotel_address TEXT NOT NULL,

    CONSTRAINT pk_hotels PRIMARY KEY (id_hotel),

    CONSTRAINT chk_hotels_name_not_empty
        CHECK (char_length(trim(hotel_name)) > 0),

    CONSTRAINT chk_hotels_class
        CHECK (hotel_class BETWEEN 1 AND 5),

    CONSTRAINT chk_hotels_address_not_empty
        CHECK (char_length(trim(hotel_address)) > 0),

-- В одной стране не должно быть двух полностью одинаково
-- описанных отелей.


CONSTRAINT uq_hotels_country_name_address
        UNIQUE (id_country, hotel_name, hotel_address),

    CONSTRAINT fk_hotels_countries
        FOREIGN KEY (id_country)
        REFERENCES countries (id_country)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);


CREATE TABLE travel_packages (
    id_package INTEGER GENERATED ALWAYS AS IDENTITY,
    id_hotel INTEGER NOT NULL,
    duration_days INTEGER NOT NULL,
    base_price NUMERIC(12, 2) NOT NULL,
    conditions TEXT NOT NULL,

    CONSTRAINT pk_travel_packages PRIMARY KEY (id_package),

-- По условию путевки продаются на одну, две или четыре недели.


CONSTRAINT chk_travel_packages_duration
        CHECK (duration_days IN (7, 14, 28)),

    CONSTRAINT chk_travel_packages_base_price
        CHECK (base_price >= 0),

    CONSTRAINT chk_travel_packages_conditions_not_empty
        CHECK (char_length(trim(conditions)) > 0),

    CONSTRAINT fk_travel_packages_hotels
        FOREIGN KEY (id_hotel)
        REFERENCES hotels (id_hotel)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);

CREATE TABLE orders (
    id_order INTEGER GENERATED ALWAYS AS IDENTITY,
    id_client INTEGER NOT NULL,
    sale_date DATE NOT NULL,
    departure_date DATE NOT NULL,
    total_cost NUMERIC(12, 2) NOT NULL,
    purchased_packages_count INTEGER NOT NULL,
    CONSTRAINT pk_orders PRIMARY KEY (id_order),
    CONSTRAINT chk_orders_total_cost CHECK (total_cost >= 0),
    CONSTRAINT chk_orders_purchased_packages_count CHECK (purchased_packages_count > 0),
    CONSTRAINT chk_orders_departure_date CHECK (departure_date >= sale_date),
    CONSTRAINT fk_orders_clients FOREIGN KEY (id_client) REFERENCES clients (id_client) ON UPDATE CASCADE ON DELETE RESTRICT
);


CREATE TABLE discounts (
    id_discount INTEGER GENERATED ALWAYS AS IDENTITY,
    discount_name VARCHAR(100) NOT NULL,
    provision_conditions TEXT NOT NULL,
    discount_percent NUMERIC(5, 2) NOT NULL,

    CONSTRAINT pk_discounts PRIMARY KEY (id_discount),

-- Альтернативный ключ из реляционной модели.


CONSTRAINT uq_discounts_discount_name UNIQUE (discount_name),

    CONSTRAINT chk_discounts_name_not_empty
        CHECK (char_length(trim(discount_name)) > 0),

    CONSTRAINT chk_discounts_conditions_not_empty
        CHECK (char_length(trim(provision_conditions)) > 0),

    CONSTRAINT chk_discounts_percent
        CHECK (discount_percent >= 0 AND discount_percent <= 100)
);

CREATE TABLE interested_packages (
    id_client INTEGER NOT NULL,
    id_package INTEGER NOT NULL,
    CONSTRAINT pk_interested_packages PRIMARY KEY (id_client, id_package),
    CONSTRAINT fk_interested_packages_clients FOREIGN KEY (id_client) REFERENCES clients (id_client) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_interested_packages_travel_packages FOREIGN KEY (id_package) REFERENCES travel_packages (id_package) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE order_packages (
    id_order INTEGER NOT NULL,
    id_package INTEGER NOT NULL,
    CONSTRAINT pk_order_packages PRIMARY KEY (id_order, id_package),
    CONSTRAINT fk_order_packages_orders FOREIGN KEY (id_order) REFERENCES orders (id_order) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_order_packages_travel_packages FOREIGN KEY (id_package) REFERENCES travel_packages (id_package) ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE order_discounts (
    id_discount INTEGER NOT NULL,
    id_order INTEGER NOT NULL,
    CONSTRAINT pk_order_discounts PRIMARY KEY (id_discount, id_order),
    CONSTRAINT fk_order_discounts_discounts FOREIGN KEY (id_discount) REFERENCES discounts (id_discount) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_order_discounts_orders FOREIGN KEY (id_order) REFERENCES orders (id_order) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE INDEX idx_clients_id_user ON clients (id_user);

CREATE INDEX idx_hotels_id_country ON hotels (id_country);

CREATE INDEX idx_travel_packages_id_hotel ON travel_packages (id_hotel);

CREATE INDEX idx_orders_id_client ON orders (id_client);

CREATE INDEX idx_interested_packages_id_package ON interested_packages (id_package);

CREATE INDEX idx_order_packages_id_package ON order_packages (id_package);

CREATE INDEX idx_order_discounts_id_order ON order_discounts (id_order);