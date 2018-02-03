create table `test`(
    `id` int auto_increment not null,  
    `type` int not null, 
    `ip` varchar(69) not null, 
    `port` int not null, 
    `status` int not null, 
    primary key(`id`), 
    unique index `tip`(`type`, `ip`,`port`)
    )engine=myisam;

