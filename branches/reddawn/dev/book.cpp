void namebook(char *str)
{
   strcpy(str,"");

switch(LCSrandom(4))
            { //XXX: BEGIN AUTOBIOGRAPHY
               case 0:strcat(str,"My Struggles As a");break;
               case 1:strcat(str,"Life As a");break;
               case 2:strcat(str,"The Struggles of a");break;
               case 3:strcat(str,"The Life of a");break;
                       switch(LCSrandom(4))
                                   {
                                           case 0:
                                                  switch(LCSrandom(4))
                                                              {
                                                                       case 0:strcat(str," Gay Man");break;
                                                                       case 1:strcat(str," Lesbian");break;
                                                                       case 2:strcat(str," Bisexual");break;
                                                                       case 3:
                                                                              switch(LCSrandom(2))
                                                                                          {
                                                                                                   case 0:strcat(str," Transsexual");break;
                                                                                                   case 1:strcat(str," Transgendered Person");break;

                                                                                                   default:strcat(str,"FOOBAR BAZ");break;
                                                                                          }

                                                                       default:strcat(str,"BAR");break;
                                                              }
                                           case 1:strcat(str,"n Illegal Immigrant");break;
                                           case 2:strcat(str,"n African American");break;
                                           case 2:strcat(str," Liberal");break;
                                           case 2:strcat(str," Woman");break;

                                           default:strcat(str,"BAZ");break;
                                   }
                       switch(LCSrandom(2))
                                   {
                                           case 0:
                                                  if(attitude[VIEW_STALIN]+LCSrandom(50)<=90)strcat(str," in a Stalinist World");
                                                  else if(law[LAW_RELIGION]==-2)strcat(str," in a God-Fearing World");
                                                  else strcat(str," in a Conservative World");break;
                                           case 1:strcat(str,"");break;

                                           default:strcat(str,"FOOBAR");break;
                                   }

        default:strcat(str,"FOO");break;
   } //XXX: END AUTOBIOGRAPHY
}

