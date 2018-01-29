create database `test`;
use `test`;




create table block(
    `id` bigint auto_increment not null,
    `height` bigint not null,
    `block_hash` varchar(128) not null,
    `pre_block_hash` varchar(128) not null, 
    `version` bigint not null, 
    `time` bigint not null,
    `bits` bigint not null,
    `nonce` bigint not null,

    `tx_count` int not null,

    `status` tinyint(3) not null,
    primary key (`id`),
    unique key(`block_hash`)
)engine=myisam;

create table tx(
   `id` bigint(20) NOT NULL AUTO_INCREMENT,
   `tx_hash` varchar(128) NOT NULL,
   `block_id` bigint(20) NOT NULL DEFAULT '0',
   `indexof_block` int(11) NOT NULL DEFAULT '0',
   `is_coinbase` tinyint(3) NOT NULL DEFAULT '0',
   `tx_in_count` int not null DEFAULT '0',
   `tx_out_count` int not null DEFAULT '0',
   `status` tinyint(3) NOT NULL,
    primary key (`id`),
    unique key  `tx_hash` (`tx_hash`),
    CONSTRAINT block_id FOREIGN KEY (block_id) REFERENCES block (id)
)engine=myisam;



create table tx_coinbase(
    `id` bigint(20) NOT NULL AUTO_INCREMENT,
    `tx_id` bigint(20) NOT NULL,
    `coinbase_hash` varchar(128) DEFAULT NULL,
    `coinbase_sequence` bigint(20) DEFAULT NULL,
    `coinbase_value` bigint(20) NOT NULL,
    `out_address` varchar(64) NOT NULL,
    `status` tinyint(3) NOT NULL,
    PRIMARY KEY (`id`),
    KEY `tx_id` (`tx_id`),
    CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
)engine=myisam;


create table tx_in(
    `id` bigint(20) NOT NULL AUTO_INCREMENT,
    `tx_id` bigint(20) NOT NULL,
    `tx_hash` varchar(128) NOT NULL,
    `vin` int(11) NOT NULL,
    `sig_hex` varchar(128) NOT NULL,
    `sequence` bigint NOT NULL,
    `from_tx_hash` varchar(65) NOT NULL,
    `from_tx_vout` int(11) NOT NULL,
    `status` tinyint(3) NOT NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `tx_in_key` (`tx_id`,`vin`),
    KEY `tx_id` (`tx_id`),
    CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
)engine=myisam;



create table tx_out(
    `id` bigint(20) NOT NULL AUTO_INCREMENT,
    `tx_id` bigint(20) NOT NULL,
    `tx_hash` varchar(128) NOT NULL,
    `vout` int(11) NOT NULL,
    `out_value` bigint(20) NOT NULL,
    `out_address` varchar(64) NOT NULL,
    `out_type` varchar(128) NOT NULL,
    `out_addresses` varchar(512) not null,
    `out_addr_count` int not null,
    `status` tinyint(3) not null,
    PRIMARY KEY (`id`),
    UNIQUE KEY `tx_out_key` (`tx_id`,`vout`),
    KEY `tx_id` (`tx_id`),
    CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
)engine=myisam;
