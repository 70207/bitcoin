create table `test`(
    `id` int auto_increment not null,  
    `type` int not null, 
    `ip` varchar(69) not null, 
    `port` int not null, 
    `status` int not null, 
    primary key(`id`), 
    unique index `tip`(`type`, `ip`,`port`)
    )engine=myisam;


create table tx(
    id bigint auto_increment not null,
    hash varchar(65) not null,
    block_hash varchar(128) not null,
    is_coinbase tinyint(3) not null,
    status tinyint(3) not null,
    primary key (`id`),
    unique key(`hash`),

)engine=myisam;

create table tx_coinbase(
    id bigint auto_increment not null,
    tx_id bigint not null,
    coinbase_hash varchar(128),
    coinbase_sequence bigint,
    coinbase_value bigint not null,
    out_address varchar(64) not null,
    primary key(id),
    KEY tx_id (tx_id),
    CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
)engine=myisam;

create table tx_in(
    id bigint auto_increment not null,
    tx_id bigint not null,
    from_tx_hash varchar(65) not null,
    vout int not null,
    primary key(id),
    KEY tx_id (tx_id),
    CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
)engine=myisam;

create table tx_out(
    id bigint auto_increment not null,
    tx_id bigint not null,
    vout int not null,
    out_value bigint not null,
    out_address varchar(64) not null,
    primary key(id),
    KEY tx_id (tx_id),
    CONSTRAINT key_id FOREIGN KEY (tx_id) REFERENCES tx (id)
)engine=myisam;