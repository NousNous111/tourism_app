DROP TABLE IF EXISTS interested_packages CASCADE;

DROP TABLE IF EXISTS order_packages CASCADE;

DROP TABLE IF EXISTS orders CASCADE;

DROP TABLE IF EXISTS travel_packages CASCADE;

DROP TABLE IF EXISTS hotels CASCADE;

DROP TABLE IF EXISTS countries CASCADE;

DROP TABLE IF EXISTS clients CASCADE;

DROP TABLE IF EXISTS users CASCADE;

CREATE TABLE users (
    id_user SERIAL PRIMARY KEY,
    login VARCHAR(100) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role VARCHAR(20) NOT NULL,
    CONSTRAINT chk_users_role CHECK (role IN ('client', 'admin'))
);

CREATE TABLE clients (
    id_client SERIAL PRIMARY KEY,
    id_user INTEGER NOT NULL UNIQUE,
    last_name VARCHAR(100) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    patronymic VARCHAR(100),
    address VARCHAR(255) NOT NULL,
    phone VARCHAR(30) NOT NULL,
    CONSTRAINT fk_clients_users FOREIGN KEY (id_user) REFERENCES users (id_user) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE countries (
    id_country SERIAL PRIMARY KEY,
    country_name VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE hotels (
    id_hotel SERIAL PRIMARY KEY,
    id_country INTEGER NOT NULL,
    hotel_name VARCHAR(150) NOT NULL,
    stars INTEGER NOT NULL,
    hotel_address VARCHAR(255) NOT NULL,
    CONSTRAINT fk_hotels_countries FOREIGN KEY (id_country) REFERENCES countries (id_country) ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT chk_hotels_stars CHECK (stars BETWEEN 1 AND 5)
);

CREATE TABLE travel_packages (
    id_package SERIAL PRIMARY KEY,
    id_hotel INTEGER NOT NULL,
    duration_days INTEGER NOT NULL,
    base_price NUMERIC(10, 2) NOT NULL,
    conditions TEXT NOT NULL,
    CONSTRAINT fk_travel_packages_hotels FOREIGN KEY (id_hotel) REFERENCES hotels (id_hotel) ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT chk_travel_packages_duration CHECK (duration_days IN (7, 14)),
    CONSTRAINT chk_travel_packages_base_price CHECK (base_price > 0)
);

CREATE TABLE orders (
    id_order SERIAL PRIMARY KEY,
    id_client INTEGER NOT NULL,
    sale_date DATE NOT NULL,
    departure_date DATE NOT NULL,
    total_cost NUMERIC(10, 2) NOT NULL,
    purchased_packages_count INTEGER NOT NULL,
    discount_percent NUMERIC(5, 2) NOT NULL DEFAULT 0,
    CONSTRAINT fk_orders_clients FOREIGN KEY (id_client) REFERENCES clients (id_client) ON UPDATE CASCADE ON DELETE RESTRICT,
    CONSTRAINT chk_orders_total_cost CHECK (total_cost >= 0),
    CONSTRAINT chk_orders_purchased_packages_count CHECK (purchased_packages_count > 0),
    CONSTRAINT chk_orders_discount_percent CHECK (
        discount_percent >= 0
        AND discount_percent <= 100
    ),
    CONSTRAINT chk_orders_dates CHECK (departure_date >= sale_date)
);

CREATE TABLE order_packages (
    id_order INTEGER NOT NULL,
    id_package INTEGER NOT NULL,
    CONSTRAINT pk_order_packages PRIMARY KEY (id_order, id_package),
    CONSTRAINT fk_order_packages_orders FOREIGN KEY (id_order) REFERENCES orders (id_order) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_order_packages_travel_packages FOREIGN KEY (id_package) REFERENCES travel_packages (id_package) ON UPDATE CASCADE ON DELETE RESTRICT
);

CREATE TABLE interested_packages (
    id_client INTEGER NOT NULL,
    id_package INTEGER NOT NULL,
    CONSTRAINT pk_interested_packages PRIMARY KEY (id_client, id_package),
    CONSTRAINT fk_interested_packages_clients FOREIGN KEY (id_client) REFERENCES clients (id_client) ON UPDATE CASCADE ON DELETE CASCADE,
    CONSTRAINT fk_interested_packages_travel_packages FOREIGN KEY (id_package) REFERENCES travel_packages (id_package) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE INDEX idx_clients_id_user ON clients (id_user);

CREATE INDEX idx_hotels_id_country ON hotels (id_country);

CREATE INDEX idx_travel_packages_id_hotel ON travel_packages (id_hotel);

CREATE INDEX idx_orders_id_client ON orders (id_client);

CREATE INDEX idx_order_packages_id_order ON order_packages (id_order);

CREATE INDEX idx_order_packages_id_package ON order_packages (id_package);

CREATE INDEX idx_interested_packages_id_client ON interested_packages (id_client);

CREATE INDEX idx_interested_packages_id_package ON interested_packages (id_package);